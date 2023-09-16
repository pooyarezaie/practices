package XType;

public class UnknownType  extends Type {

    public boolean isUnknown() {
        return true;
    }

    public boolean isInteger() {
        return true;
    }

    public boolean isCharacter() {
        return true;
    }

    public boolean isVoid() {
        return false;
    }

    public boolean hasLazy() {
        return false;
    }

    @Override
    public String toString() {
        return "Unknown Type";
    }

    @Override
    public int hashCode() {
        return 1;
    }

    @Override
    public boolean equals(Object o) {
        return o instanceof Type;
    }
}
