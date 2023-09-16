package Xlang;


public class OperationManager {
    public enum OperationType {
        ADD, SUB, MULTIPLY, DIVIDE,
        AND, OR,
        GT, LT,
        EQ, NEQ,
        SHARP,
        NOTHING
    }
    public static OperationType createType(String op) {
        switch (op) {
            case "+":
                return OperationType.ADD;
            case "-":
                return OperationType.SUB;
            case "/":
                return OperationType.DIVIDE;
            case "*":
                return OperationType.MULTIPLY;
            case "&&":
                return OperationType.AND;
            case "||":
                return OperationType.OR;
            case ">":
                return OperationType.GT;
            case "<":
                return OperationType.LT;
            case "==":
                return OperationType.EQ;
            case "!=":
                return OperationType.NEQ;
            case "#":
                return OperationType.SHARP;
            default:
                return OperationType.NOTHING;
        }
    }
}
