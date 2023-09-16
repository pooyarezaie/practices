package Xlang;

import XType.Type;

public class VariableInfo {
    private Type type;
    private int offset;

    public VariableInfo(Type type, int offset) {
        this.type = type;
        this.offset = offset;
    }

    public VariableInfo freshType(String typeName, Type typeValue) {
        type.freshType(typeName, typeValue);
        return this;
    }

    public Type getType() {
        return type;
    }
    public int getOffset() {
        return offset;
    }

    @Override
    public String toString() {
        return "Type : " + type + " Size : " + type.getSize() + " Offset : " + offset;
    }
}
