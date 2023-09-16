package XType;

import Utils.Pair;

import java.util.ArrayList;
import java.util.stream.Collectors;

public class ListType extends Type {

    private ArrayList<Type> elementsType = new ArrayList<>();
    private ArrayList<String> elementsName = new ArrayList<>();

    public ListType(ArrayList<Pair<Type, String>> blocks) {
        for (Pair<Type, String> block :blocks) {
            elementsType.add(block.getLeft());
            elementsName.add(block.getRight());
        }
    }

    public ListType(ArrayList<Type> elementsType, ArrayList<String> elementsName) {
        this.elementsType = elementsType;
        if (elementsName != null)
            this.elementsName = elementsName;
    }

    public ListType(Type type, int count){
        for (int i = 0; i < count; ++i) {
            elementsType.add(type);
            elementsName.add(null);
        }
    }

    public Type getField(String fieldName) {
        for (int i = 0; i < elementsName.size(); ++i) {
            if (elementsName.get(i)!= null && elementsName.get(i).equals(fieldName)) {
                return elementsType.get(i);
            }
        }
        return new UnknownType();
    }

    public boolean hasField(String fieldName) {
        for (String name: elementsName)
            if (name.equals(fieldName))
                return true;
        return false;
    }

    public Type isHomogeneous() {
        Type type = null;

        for (Type elementType : elementsType) {
            if (type != null && !elementType.equals(type))
                return null;
            type = elementType;
        }
        return type;
    }

    public boolean isString() {
        Type type = isHomogeneous();
        return (type != null) && type.isCharacter();
    }

    public Type getField(int index) {
        return elementsType.get(index);
    }

    public int getFieldOffset(int index) {
        int offsetFromBase = 0;
        for (int baseIndex = 0; baseIndex < index; baseIndex++) {
            offsetFromBase += elementsType.get(baseIndex).getSize();
        }
        return offsetFromBase;
    }

    public int getFieldOffset(String fieldName) {
        int offsetFromBase = 0;
        int baseIndex = 0;
        while (!elementsName.get(baseIndex).equals(fieldName)) {
            offsetFromBase += elementsType.get(baseIndex).getSize();
            baseIndex++;
        }
        return offsetFromBase;
    }

    public ArrayList<Type> getElementsType() {
        return elementsType;
    }
    public ArrayList<String> getElementsName() {
        return elementsName;
    }

    @Override
    public int getSize() {
        int size = 0;
        for (Type type: elementsType)
            size += type.getSize();
        return size;
    }

    @Override
    public boolean hasLazy() {
        for (Type type : elementsType)
            if (type.hasLazy())
                return true;
        return false;
    }

    @Override
    public Type freshType(String typeName, Type typeValue) {
        elementsType = elementsType.stream().map(element -> element.freshType(typeName, typeValue))
                .collect(Collectors.toCollection(ArrayList::new));
        return this;
    }

    public ListType updateElementsType(ArrayList<Type> elementsType) {
        this.elementsType = elementsType;
        return this;
    }

    public int getLength() {
        return elementsType.size();
    }

    public Type getLastElementType() {
        return elementsType.get(elementsType.size()-1);
    }

    public Type getFirstElementType() {
        return elementsType.get(0);
    }

    @Override
    public int hashCode() {
        int result = super.hashCode();
        result = 31 * result + (elementsType != null ? elementsType.hashCode() : 0);
        return result;
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Type && ((Type) o).isUnknown())
            return true;
        if (!(o instanceof ListType))
            return false;
        ListType listType = (ListType) o;

        if (elementsType.size() != listType.elementsType.size())
            return false;

        for (int i = 0; i < elementsType.size(); ++i)
            if (!elementsType.get(i).equals(listType.elementsType.get(i)))
                return false;
        return true;
    }

    @Override
    public String toString() {
        return '[' + elementsType.stream().map(Type::toString)
                .collect(Collectors.joining(", ")) + ']';
    }
}
