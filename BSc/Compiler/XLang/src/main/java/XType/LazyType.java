package XType;

public class LazyType extends Type {
    private String name;

    public LazyType(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    @Override
    public Type freshType(String typeName, Type TypeValue) {
        if (name.equals(typeName) && TypeValue != null)
            return TypeValue;
        return this;
    }

    @Override
    public boolean hasLazy() {
        return true;
    }

    @Override
    public String toString() {
        return name + "(?)";
    }
}
