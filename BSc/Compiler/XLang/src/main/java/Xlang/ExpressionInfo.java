package Xlang;

import java.util.ArrayList;
import java.util.stream.Collectors;

import XType.Type;
import XType.ListType;
import XType.PrimitiveType;

public class ExpressionInfo {
    private Type type;
    private boolean assignable;
    // TODO : think of better name for variableName (maybe baseVariableName)
    private String variableName;
    /* for list access we should know what is the offset from beginning of list */
    private int offsetFromBase = 0;

    public ExpressionInfo(Type type, boolean assignable, String variableName) {
        this.type = type;
        this.assignable = assignable;
        this.variableName = variableName;
    }

    public ExpressionInfo(Type type, boolean assignable) {
        this(type, assignable, null);
    }

    public ExpressionInfo(Type type, String variableName) {
        this(type, true, variableName);
    }

    public ExpressionInfo(Type type) {
        this(type, true, null);
    }

    public ExpressionInfo(Type type, boolean assignable, String variableName, int offsetFromBase) {
        this(type, assignable, variableName);
        this.offsetFromBase = offsetFromBase;
    }

    public static ExpressionInfo createInteger() {
        return new ExpressionInfo(PrimitiveType.createInteger());
    }

    public static ExpressionInfo createCharacter() {
        return new ExpressionInfo(PrimitiveType.createCharacter());
    }

    public static ExpressionInfo createList(ArrayList<ExpressionInfo> expressions) {
        return new ExpressionInfo(new ListType(expressions
                .stream().map(ExpressionInfo::getType)
                .collect(Collectors.toCollection(ArrayList::new)), null));
    }

    public static ExpressionInfo createString(String str) {
        return new ExpressionInfo(new ListType(PrimitiveType.createCharacter(), normalizeString(str).length()));
    }

    private static String normalizeString(String abnormal) {
        return abnormal.replaceAll("\\\\n", "\n")
                .replaceAll("\\\\t", "\t")
                .replaceAll("\\\\r", "\r");
    }

    public Type getType() {
        return type;
    }

    public boolean isAssignable() {
        return assignable;
    }

    public  boolean isInteger() {
        return type.isInteger();
    }

    public boolean isVariable() {
        return variableName != null;
    }

    public String getVariableName() {
        return variableName;
    }

    public int getBaseOffset() {
        return offsetFromBase;
    }

    public boolean isWriteBuiltInFunction() {
        return variableName.equals("write");
    }

    public boolean isReadBuiltInFunction() {
        return variableName.equals("read");
    }

    public String showType() {
        return type.toString();
    }
}
