package XType;

import Xlang.ExpressionInfo;

import java.util.ArrayList;
import java.util.stream.Collectors;

public class PTFType extends Type {
    private ArrayList<Type> arguments;
    private Type returnType;
    private String functionName;

    public PTFType(ArrayList<Type> arguments, Type returnType) {
        this.arguments = arguments;
        this.returnType = returnType;
        if (this.arguments.isEmpty())
            this.arguments.add(PrimitiveType.createVoid());
        functionName = null;
    }

    public PTFType(String functionName) {
        this.functionName = functionName;
    }

    public boolean isBuiltIn() {
        return functionName != null;
    }

    public Type getReturnType() {
        return returnType;
    }

    public PTFType updateReturnType(Type returnType) {
        this.returnType = returnType;
        return this;
    }

    public ArrayList<Type> getArguments() {
        return arguments;
    }

    public PTFType updateArguments(ArrayList<Type> arguments) {
        this.arguments = arguments;
        return this;
    }

    public String showArguments() {
        return "(" + arguments.stream().map(Object::toString)
                .collect(Collectors.joining(",")) + ")";
    }

    @Override
    public boolean hasLazy() {
        for (Type type : arguments)
            if (type.hasLazy())
                return true;
        return returnType.hasLazy();
    }

    @Override
    public Type freshType(String typeName, Type typeValue) {
        arguments = arguments.stream().map(argument -> argument.freshType(typeName, typeValue))
                .collect(Collectors.toCollection(ArrayList::new));
        returnType = returnType.freshType(typeName, typeValue);
        return this;
    }

    @Override
    public int getSize() {
        int size = 0;
        for (Type argument : arguments)
            size += argument.getSize();
        return size;
    }

    public boolean isCompatibleArguments(ArrayList<ExpressionInfo> arguments) {
        boolean compatible = (this.arguments.size() == arguments.size());

        for (int i = 0; i < Integer.min(this.arguments.size(), arguments.size()); ++i) {
            if (!this.arguments.get(i).equals(arguments.get(i).getType())) {
                compatible = false;
            }
        }
        return compatible;
    }

    @Override
    public int hashCode() {
        int result = super.hashCode();
        result = 31 * result + (arguments != null ? arguments.hashCode() : 0);
        result = 31 * result + (returnType != null ? returnType.hashCode() : 0);
        return result;
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Type && ((Type) o).isUnknown())
            return true;
        if (!(o instanceof PTFType))
            return false;
        PTFType ptfType = (PTFType) o;

        return this.arguments.equals(ptfType.arguments) &&
                this.returnType.equals(ptfType.returnType);
    }

    @Override
    public String toString() {
        return '<' + arguments.stream().map(Object::toString)
                .collect(Collectors.joining("*")) + "->" + returnType.toString() + '>';
    }

}
