package Xlang;

import XType.Type;
import Utils.Pair;

import java.util.ArrayList;
import java.util.HashMap;

public class Environment {

    private Environment parent;
    private SymbolTable table;
    private ErrorManager errorManager;
    private ScopeType scopeType;

    public Environment(Environment parent, ScopeType scopeType, ArrayList<Pair<String, Type>>
            initialScopeArguments, ErrorManager errorManager) {
        this.parent = parent;
        this.errorManager = errorManager;
        this.table = new SymbolTable(errorManager, initialScopeArguments);
        this.scopeType = scopeType;
    }

    public Environment getParent() {
        return parent;
    }

    public int getScopeSize() {
        return table.getSize();
    }

    public int getTotalScopeSize() {
        int scopeSize = getScopeSize();
        if (parent != null) {
            scopeSize += parent.getTotalScopeSize();
        }
        return scopeSize;
    }

    public ScopeType getScopeType() {
        return scopeType;
    }

    public VariableInfo getVariableInfo(String variableName) {
        VariableInfo info = table.getVariableInfo(variableName);
        if (info == null && parent != null)
            info = parent.getVariableInfo(variableName);
        return info;
    }

    public void hookUpdateTypes(HashMap<String, Type> typeTable) {
        table.hookUpdateTypes(typeTable);
        if (parent != null)
            parent.hookUpdateTypes(typeTable);
    }

    public void addVariable(String variableName, Type type) {
        table.addVariable(variableName, type);
    }

    public boolean hasVariable(String variableName) {
        return table.hasVariable(variableName) || parent != null && parent.hasVariable(variableName);
    }

    public int getVariableOffset(String variableName) {
        if (table.hasVariable(variableName)) {
            if (parent != null)
                return parent.getTotalScopeSize() + table.getVariableOffset(variableName);
            else
                return table.getVariableOffset(variableName);
        }
        return parent.getVariableOffset(variableName);
    }

    public void updateVariable(String variableName, ExpressionInfo expressionInfo) {
        if (table.hasVariable(variableName)) {
            Type type = table.getVariableInfo(variableName).getType();
            if (!type.equals(expressionInfo.getType())) {
                errorManager.error("Mismatch type while assigning " + type + ".");
                System.out.println("Invalid conversion from " + expressionInfo.getType() + " to " + type + ".");
            }
        }
        else if (parent != null) {
            parent.updateVariable(variableName, expressionInfo);
        }
        else {
            errorManager.error("Variable " + variableName + " is not declared before.");
        }
    }

    public boolean canBreak() {
        if ((scopeType == ScopeType.WHILE) ||
                (scopeType == ScopeType.FOREACH)) {
            return true;
        }
        return parent != null && parent.canBreak();
    }
}
