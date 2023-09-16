package Xlang;

import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import Antlr.XParser;
import XType.*;
import Utils.Pair;


public class ProgramManager {
    private Environment currentEnv;
    private static ErrorManager errorManager;
    private static CodeGenerator codeGenerator;
    private TypeManager typeManager;

    public ProgramManager() {
        errorManager = new ErrorManager();
        codeGenerator = new CodeGenerator();
        this.currentEnv = new Environment(null, ScopeType.GLOBAL, new ArrayList<>(), errorManager);
        typeManager = new TypeManager(errorManager);
    }

    public void setParser(XParser parser) {errorManager.setParser(parser);}

    public ErrorManager getErrorManager() {return errorManager;}


    /* scopes */

    public void handleBeginScope(ArrayList<Pair<String, Type>> arguments, ScopeType scopeType) {
        currentEnv= new Environment(currentEnv, scopeType, arguments, errorManager);
    }

    public void handleEndScope() {
        codeGenerator.popScope(currentEnv.getScopeSize());
        currentEnv = currentEnv.getParent();
    }

    public void handleBreakableScope() { //todo: must be handled
        if (!currentEnv.canBreak())
            errorManager.error("Cannot break from non while(foreach) scope.");
    }

    public void handleReturnStatement(ExpressionInfo returnExpression) {
        System.out.println("Handle return " + returnExpression.getType());
    }


    /* flow controls */

    public Pair<String, String> handleForeachStatement(Type variableType, String variableName,
                                                       ExpressionInfo expressionInfo) {
        codeGenerator.commit();
        if (!(expressionInfo.getType() instanceof ListType))
            errorManager.error("Cannot iterate over non list variable.");
        else {
            Type type = ((ListType) expressionInfo.getType()).isHomogeneous();
            if (type == null)
                errorManager.error("All elements in list must have the same type.");
            else if (!variableType.equals(type)) {
                errorManager.error(String.format(
                        "Iterate variable type (%s) must be the same with list elements.", variableName));
                System.out.println("Given " + variableType + " Expected " + type);
            }
            else
                return codeGenerator.handleForeachBranch(variableType.getSize(), currentEnv.getTotalScopeSize());
        }
        return new Pair<>("", "");
    }

    public void handleBeginForeach(int variableSize) {
        codeGenerator.handleBeginForeach(variableSize);
    }

    public void handleEndForeach(Type type, ExpressionInfo expressionInfo, Pair<String, String> labels) {
        codeGenerator.handleEndForeach(type.getSize(), ((ListType) expressionInfo.getType()).getLength(), labels);
    }

    public void handleConditionType(ExpressionInfo info) {
        if (!info.isInteger())
            errorManager.error("Conditional's expressions should be integer.");
    }

    public String handleBeginWhile() {
        return codeGenerator.handleBeginWhile();
    }

    public String handleWhileBranch() {
        codeGenerator.commit();
        return codeGenerator.handleWhileBranch();
    }

    public void handleEndWhile(String startLabel, String endLabel) {
        codeGenerator.handleEndWhile(startLabel, endLabel);
    }

    public String handleBeginIf(ExpressionInfo info) {
        handleConditionType(info);
        return codeGenerator.handleBeginIf();
    }

    public void handleEndIf(String ifLabel) {
        codeGenerator.handleEndIf(ifLabel);
    }

    public Pair<String, String> handleBeginIfElse(ExpressionInfo info)  {
        handleConditionType(info);
        return codeGenerator.handleBeginIfElse();
    }

    public void handleBeginElse(String ifLabel, String elseLabel) {
        codeGenerator.handleBeginElse(ifLabel, elseLabel);
    }

    /*
    variables
     */

    public boolean isReservedWord(String name) {
        List<String> reservedWords = Arrays.asList("int", "char", "void");

        return reservedWords.contains(name);
    }

    public void handleVariableAssignment(String variableName, ExpressionInfo expressionInfo) {
        currentEnv.updateVariable(variableName, expressionInfo);
    }

    public void handleVariableDeclaration(Type type, String variableName, ExpressionInfo expressionInfo) {
        codeGenerator.commit();
        if (isReservedWord(variableName))
            errorManager.error("Variable name '" + variableName + " is in reserved words.");
        else {
            currentEnv.addVariable(variableName, type);
            if (expressionInfo != null)
                handleVariableAssignment(variableName, expressionInfo);
            else
                codeGenerator.pushVariable(type.getSize());
        }
    }

    public void handleVariableDeclaration(Type type, Pair<String, ExpressionInfo> assignment) {
        handleVariableDeclaration(type, assignment.getLeft(), assignment.getRight());
    }

    /*
    types
     */

    public Type getEquivalentType(String typeName) {
        return typeManager.getEquivalentType(typeName);
    }

    public void handleTypedefForwardDeclaration(String typeName) {
        if (currentEnv.getScopeType() != ScopeType.GLOBAL) {
            errorManager.error("Typedefs can only declared in global scope.");
        }
        else {
            typeManager.handleTypedefForwardDeclaration(typeName);
        }
    }

    public void handleTypedefStatement(String name, Type type) {
        if (currentEnv.getScopeType() != ScopeType.GLOBAL) {
            errorManager.error("Typedefs can only declared in global scope.");
        }
        else {
            typeManager.handleTypedefStatement(name, type);
            currentEnv.hookUpdateTypes(typeManager.getTypeTable());
        }
    }

    /*
    handle function
     */

    public void handleFunctionDefinition(Type returnType, String functionName,
                                         ArrayList<Pair<String, Type>> arguments) {
        if (currentEnv.getScopeType() != ScopeType.GLOBAL) {
            errorManager.error("Functions can only defined in global scope.");
        }
        else {
            boolean illegalMain = false;
            if (functionName.equals("main")) {
                if (!returnType.isVoid()) {
                    illegalMain = true;
                    errorManager.error("Function main should return void.");
                }
                if (arguments.size() != 0) {
                    illegalMain = true;
                    errorManager.error("Function main can have only zero arguments");
                }
            }
            if (!illegalMain) {
                ArrayList<Type> argumentsType = arguments.stream().map(Pair::getRight)
                        .collect(Collectors.toCollection(ArrayList::new));

                Type type = new PTFType(argumentsType, returnType);

                if (!currentEnv.hasVariable(functionName)) {
                    currentEnv.addVariable(functionName, type);
                }
            }
        }
    }

    public void handleFunctionDeclaration(Type returnType, String functionName, ArrayList<Type> arguments) {
        if (currentEnv.getScopeType() != ScopeType.GLOBAL) {
            errorManager.error("Functions can only declared in global scope.");
        }
        else {
            if (functionName.equals("main")) {
                errorManager.error("Function main cannot forward declared.");
            }
            else {
                Type type = new PTFType(arguments, returnType);
                if (type.isUnknown())
                    errorManager.error("Invalid function declaration. " +
                            "void should only used in return type or with functions which has no inputs.");
                currentEnv.addVariable(functionName, type);
            }
        }
    }

    /*
    handle operations
     */

    public void handleIntegerConstant(int value) {
        codeGenerator.loadIntegerConstant(value);
    }

    private static String normalizeString(String abnormal) {
        return abnormal.replaceAll("\\\\n", "\n")
                .replaceAll("\\\\t", "\t")
                .replaceAll("\\\\r", "\r");
    }

    public void handleCharacterConstant(String value) {
        codeGenerator.loadCharacterConstant(normalizeString(value).charAt(1));
    }

    public void handleStringConstant(String value) {
        codeGenerator.loadStringConstant(normalizeString(value));
    }

    public ExpressionInfo createVariableExpressionInfo(String variableName) {
        List<String> builtInFunctions = Arrays.asList("write", "read");

        if (builtInFunctions.contains(variableName))
            return new ExpressionInfo(new PTFType(variableName), variableName);

        if (currentEnv.hasVariable(variableName)) {
            VariableInfo variableInfo = currentEnv.getVariableInfo(variableName);
            codeGenerator.manualCommit();
            codeGenerator.copyVariableToTop(currentEnv.getVariableOffset(variableName),
                    variableInfo.getType().getSize());
            return new ExpressionInfo(variableInfo.getType(), variableName);
        }
        else {
            errorManager.error("Variable " + variableName + " is not declared before.");
            return new ExpressionInfo(new UnknownType());
        }
    }

    public ExpressionInfo handleFunctionCall(ExpressionInfo func, ArrayList<ExpressionInfo> args) {
        codeGenerator.commit();

        int argSize = args.stream().map(ExpressionInfo::getType).map(Type::getSize)
                .mapToInt(Integer::intValue).sum();

        if (func.getType() instanceof PTFType) {
            if (func.isVariable()) {
                if (func.isWriteBuiltInFunction()) {
                    if (!args.get(0).getType().isUnknown())
                        codeGenerator.writeConsole(((ListType) args.get(0).getType()).getLength());
                    else
                        codeGenerator.writeConsole();
                    codeGenerator.shrinkStack(0, argSize);
                    return new ExpressionInfo(PrimitiveType.createVoid());
                }
                if (func.isReadBuiltInFunction()) {
                    codeGenerator.readConsole();
                    return new ExpressionInfo(new UnknownType());
                }
            }
            PTFType ptfType = (PTFType) func.getType();
            if (ptfType.isCompatibleArguments(args)) {
                codeGenerator.shrinkStack(ptfType.getReturnType().getSize(), argSize);
                return new ExpressionInfo(ptfType.getReturnType());
            }
            else {
                errorManager.error("Incompatible function call arguments.");
                System.out.println("Expected: " + ptfType.showArguments());
            }
        }
        else
            errorManager.error("Type " + func.getType() + " can not be called.");

        return new ExpressionInfo(new UnknownType());
    }

    public ExpressionInfo handleExpressionAssignment(ExpressionInfo left, ExpressionInfo right) {
        codeGenerator.commit();
        if (left == null || right == null)
            return new ExpressionInfo(new UnknownType());

        if (!left.isAssignable()) {
            errorManager.error("Assignment's left expression should be assignable.");
        }
        else {
            Type leftType = left.getType(), rightType = right.getType();
            if (!leftType.equals(rightType))
                errorManager.error("Invalid conversion from " + rightType + " to " + leftType + ".");
            else {
                codeGenerator.updateVariable
                        (left.getBaseOffset() + currentEnv.getVariableOffset(left.getVariableName()),
                                left.getType().getSize());
            }
        }
        return right;
    }

    public void handleLeftAssignment() {
        codeGenerator.roleBack();
    }

    public void handleOperation(OperationManager.OperationType operationType) {
        codeGenerator.commit();
        switch (operationType) {
            case ADD:
                codeGenerator.addOperation();
                break;
            case SUB:
                codeGenerator.subOperation();
                break;
            case MULTIPLY:
                codeGenerator.multiplyOperation();
                break;
            case DIVIDE:
                codeGenerator.divideOperation();
                break;
            case AND:
                codeGenerator.andOperation();
                break;
            case OR:
                codeGenerator.orOperation();
                break;
            case GT:
                codeGenerator.gtComparison();
                break;
            case LT:
                codeGenerator.ltComparison();
                break;
            case EQ:
                codeGenerator.eqComparison();
                break;
            case NEQ:
                codeGenerator.neqComparison();
                break;
            default:
                break;
        }
    }

    public ExpressionInfo handleLogicalOperation(ExpressionInfo leftExpression, ExpressionInfo rightExpression,
                                                 String op) {
        if (!leftExpression.isInteger() || !rightExpression.isInteger()) {
            errorManager.error("Logical operation: operands should be integers.");
            System.out.println(leftExpression.showType() + ", " + rightExpression.showType());
        }
        handleOperation(OperationManager.createType(op));
        return new ExpressionInfo(PrimitiveType.createInteger(), false);
    }

    public ExpressionInfo handleEqualityOperation(ExpressionInfo leftExpression, ExpressionInfo rightExpression,
                                                  String op) {
        if (!leftExpression.getType().equals(rightExpression.getType())) {
            errorManager.error("Equality operation: operands should have identical types.");
            System.out.println(leftExpression.showType() + ", " + rightExpression.showType());
        }
        handleOperation(OperationManager.createType(op));
        return new ExpressionInfo(PrimitiveType.createInteger(), false);
    }

    public ExpressionInfo handleComparativeOperation(ExpressionInfo leftExpression, ExpressionInfo rightExpression,
                                                     String op) {
        if (!leftExpression.isInteger() || !rightExpression.isInteger()) {
            errorManager.error("Comparative operation: operands should be integers.");
            System.out.println(leftExpression.showType() + ", " + rightExpression.showType()) ;
        }
        handleOperation(OperationManager.createType(op));
        return new ExpressionInfo(PrimitiveType.createInteger(), false);
    }

    public ExpressionInfo handleNumericOperation(ExpressionInfo leftExpression, ExpressionInfo rightExpression,
                                                 String op) {
        if (!leftExpression.isInteger() || !rightExpression.isInteger()) {
            errorManager.error("Numeric operation: operands should be integers.");
            System.out.println(leftExpression.showType() + ", " + rightExpression.showType());
        }
        handleOperation(OperationManager.createType(op));
        return new ExpressionInfo(PrimitiveType.createInteger(), false);
    }

    public ExpressionInfo handleUnaryOperation(ExpressionInfo expressionInfo, String op) {
        codeGenerator.commit();
        if (!expressionInfo.isInteger()) {
            errorManager.error("Unary operation: operand should be integer.");
            System.out.println(expressionInfo.showType());
        }
        if (op.equals("-")) {
            codeGenerator.invertOperation();
        }
        else {
            codeGenerator.notOperation();
        }
        return new ExpressionInfo(PrimitiveType.createInteger(), false);
    }

    public ExpressionInfo handleListDotOperation(ExpressionInfo listInfo, String fieldName) {
        Type listInfoType = listInfo.getType();
        if (listInfoType instanceof ListType) {
            Type fieldType = ((ListType) listInfoType).getField(fieldName);
            if (fieldType.isUnknown())
                errorManager.error("List dot operation: list doesn't have the field " + fieldName + ".");
            else {
                codeGenerator.pushVariable(4);
                codeGenerator.listAccessOperation(listInfo.getType().getSize(),
                        ((ListType) listInfoType).getFieldOffset(fieldName),
                        (fieldType.getSize()));
                return new ExpressionInfo(fieldType, listInfo.isAssignable(),
                        listInfo.getVariableName(), listInfo.getBaseOffset() +
                        ((ListType) listInfoType).getFieldOffset(fieldName));
            }
        }
        return new ExpressionInfo(new UnknownType(), listInfo.isAssignable());
    }

    public ExpressionInfo handleListSharpOperation(ExpressionInfo listInfo, int index) {
        Type listInfoType = listInfo.getType();
        if (listInfoType instanceof ListType) {
            if (index >= ((ListType) listInfoType).getLength()) {
                errorManager.error("List # operation : index out of range.");
            } else {
                codeGenerator.listAccessOperation(listInfo.getType().getSize(),
                        ((ListType) listInfoType).getFieldOffset(index),
                        ((ListType) listInfoType).getField(index).getSize());
                return new ExpressionInfo(((ListType) listInfoType).getField(index), listInfo.isAssignable(),
                        listInfo.getVariableName(), listInfo.getBaseOffset() +
                        ((ListType) listInfoType).getFieldOffset(index));
            }
        } else if (!listInfoType.isUnknown()) {
            errorManager.error("List # operation : operand should be list.");
        }
        return new ExpressionInfo(new UnknownType(), listInfo.isAssignable());
    }

    public void handleExpressionStatement(ExpressionInfo info) {
        codeGenerator.commit();
        codeGenerator.popScope(info.getType().getSize());
    }

    public void generateCode() {
        try {
            codeGenerator.saveProgram();
        }
        catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
