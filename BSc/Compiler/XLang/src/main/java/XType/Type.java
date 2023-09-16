package XType;

public abstract class Type {

    public boolean isUnknown() {
        return false;
    }

    public boolean isInteger() {
        return false;
    }

    public boolean isCharacter() {
        return false;
    }

    public boolean isVoid() {
        return false;
    }

    public boolean hasLazy() {
        return false;
    }

    public int getSize() {
        return 0;
    }

    public Type freshType(String typeName, Type typeValue) {
        return this;
    }

    @Override
    public int hashCode() {
        return 0;
    }
}
