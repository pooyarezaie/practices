package XType;

public class PrimitiveType extends Type {
    private String name;

    public PrimitiveType(String name) {
        this.name = name;
    }

    public static PrimitiveType createVoid() {
        return new PrimitiveType("void");
    }

    public static PrimitiveType createInteger() {
        return new PrimitiveType("int");
    }

    public static PrimitiveType createCharacter() {
        return new PrimitiveType("char");
    }

    @Override
    public boolean isInteger() {
        return this.name.equals("int");
    }

    @Override
    public boolean isCharacter() {
        return this.name.equals("char");
    }

    @Override
    public boolean isVoid() {
        return this.name.equals("void");
    }

    @Override
    public int getSize() {
        switch (name) {
            case "int" :
                return 4;
            case "char" :
                return 4;
            default :
                return 0;
        }
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Type && ((Type) o).isUnknown())
            return true;
        if (!(o instanceof PrimitiveType))
            return false;
        PrimitiveType primitiveType = (PrimitiveType) o;

        return this.name.equals(primitiveType.name);
    }

    @Override
    public int hashCode() {
        int result = super.hashCode();
        result = 31 * result + (name != null ? name.hashCode() : 0);
        return result;
    }

    @Override
    public String toString() {
        return name;
    }
}
