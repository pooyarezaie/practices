package Xlang;

import XType.*;

import java.util.*;

public class TypeManager {
    private HashMap<String, Type> typeTable;
    private HashMap<String, ArrayList<String>> typeDag;
    private ErrorManager errorManager;

    private boolean mark;

    public TypeManager(ErrorManager errorManager) {
        this.typeTable = new HashMap<>();
        this.typeDag = new HashMap<>();
        this.errorManager = errorManager;
    }

    public Type getEquivalentType(String typeName) {
        List<String> primitiveTypes  = Arrays.asList("int", "char", "void");

        if (primitiveTypes.contains(typeName)) {
            return new PrimitiveType(typeName);
        }
        else {
            if (hasType(typeName))
                return getType(typeName);
            else
                errorManager.error("Type " + typeName + " is not declared before.");
            return new UnknownType();
        }
    }

    public void handleTypedefForwardDeclaration(String typeName) {
        if (hasType(typeName)) {
            errorManager.error("Type " + typeName + " is declared before.");
        }
        else {
            typeTable.put(typeName, null);
            typeDag.put(typeName, new ArrayList<>());
        }
    }

    public HashMap<String, Type> getTypeTable() {return typeTable;}

    public void handleTypedefStatement(String name, Type type) {
        if (hasType(name) && (typeTable.get(name) != null)) {
                errorManager.error("Type " + name + " is declared before.");
        }
        else {
            typeTable.put(name, type);
            typeDag.putIfAbsent(name, new ArrayList<>());

            getLazies(type).stream().filter(lazy -> !typeDag.get(lazy).contains(name))
                    .forEach(lazy -> typeDag.get(lazy).add(name));

            mark = false;
            if (!checkForLoop(name, name))
                updateChildren(name);
            else {
                errorManager.error("Typedef loop detected when defining " + name);
                getLazies(type).stream().filter(lazy -> typeDag.get(lazy).contains(name))
                        .forEach(lazy -> typeDag.get(lazy).remove(name));
                typeTable.put(name, null);
                typeDag.put(name, new ArrayList<>());
            }
        }
    }

    private void updateChildren(String name) {
        typeDag.get(name).forEach(this::updateChild);
    }

    private void updateChild(String childName) {
        Type type = typeTable.get(childName);
        Type nextType = updateChild(type);
        typeTable.put(childName, nextType);
        updateChildren(childName);
    }

    private Type updateChild(Type type) {
        if (type instanceof LazyType) {
            return getType(((LazyType) type).getName());
        }
        else if (type instanceof ListType) {
            ArrayList<Type> elementsType = ((ListType) type).getElementsType();
            ArrayList<Type> nextElementsType = new ArrayList<>();
            for (Type nextType : elementsType) {
                nextElementsType.add(updateChild(nextType));
            }
            return ((ListType) type).updateElementsType(nextElementsType);
        }
        else if (type instanceof PTFType) {
            ArrayList<Type> arguments= ((PTFType) type).getArguments();
            ArrayList<Type> nextArguments = new ArrayList<>();
            for (Type argument : arguments) {
                nextArguments.add(updateChild(argument));
            }
            return ((PTFType) type).updateReturnType(updateChild(((PTFType) type).getReturnType()))
                    .updateArguments(nextArguments);
        }
        return type;
    }

    private ArrayList<String> getLazies(Type type) {
        Set<String> lazies = new HashSet<>();
        if (type instanceof LazyType) {
            lazies.add(((LazyType) type).getName());
        }
        else if (type instanceof ListType) {
            ArrayList<Type> elementsType = ((ListType) type).getElementsType();
            for (Type nextType : elementsType) {
                lazies.addAll(getLazies(nextType));
            }
        }
        else if (type instanceof PTFType) {
            ArrayList<Type> arguments = ((PTFType) type).getArguments();
            for (Type argument : arguments) {
                lazies.addAll(getLazies(argument));
            }
            lazies.addAll(getLazies(((PTFType) type).getReturnType()));
        }
        return new ArrayList<>(lazies);
    }

    private boolean hasType(String typeName) {return typeTable.containsKey(typeName);}

    private Type getType(String typeName) {
        if (hasType(typeName)) {
            if (typeTable.get(typeName) != null)
                return typeTable.get(typeName);
            return new LazyType(typeName);
        }
        return new UnknownType();
    }

    private boolean checkForLoop(String name, String start) {
        if (mark && name.equals(start))
            return true;
        mark = true;
        for (String childName : typeDag.get(name)) {
            if (checkForLoop(childName, start))
                return true;
        }
        return false;
    }
}
