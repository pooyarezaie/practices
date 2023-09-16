package Xlang;

import XType.Type;
import Utils.Pair;

import java.util.*;

public class SymbolTable {
    private HashMap<String, VariableInfo> variableTable = new HashMap<>();
    private ErrorManager errorManager;
    private int currentSize = 0;

    public SymbolTable(ErrorManager errorManager, ArrayList<Pair<String, Type>> scopeInitialArguments) {
        this.errorManager = errorManager;
        scopeInitialArguments.forEach(argument->addVariable(argument.getLeft(), argument.getRight()));
    }

    public int getSize() {
        return currentSize;
    }

    public void addVariable(String variableName, Type type) {
        if (type.hasLazy())
            errorManager.error("Type " + type + " is not clearly defined yet");
        if (hasVariable(variableName))
            errorManager.error(String.format("Variable %s has already declared." , variableName));

        VariableInfo info = new VariableInfo(type, currentSize);
        variableTable.put(variableName, info);
        currentSize += info.getType().getSize();
    }

    public int getVariableOffset(String variableName) {
        return variableTable.get(variableName).getOffset();
    }

    public VariableInfo getVariableInfo(String variableName) {
        return variableTable.get(variableName);
    }

    public void hookUpdateTypes(HashMap<String, Type> typeTable) {
        for (Map.Entry<String, Type> typeEntry : typeTable.entrySet())
            for (Map.Entry<String, VariableInfo> variableEntry : variableTable.entrySet())
                variableEntry.getValue().freshType(typeEntry.getKey(), typeEntry.getValue());
    }

    public boolean hasVariable(String variableName) {
        for (Map.Entry<String, VariableInfo> symbolEntry : variableTable.entrySet())
            if (symbolEntry.getKey().equals(variableName))
                return true;
        return false;
    }
}
