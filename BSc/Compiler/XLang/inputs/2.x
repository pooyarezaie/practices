void main()
{
    int a = 96, b = 18;
    int c;
    while (b != 0) {
        // a, b -> b, a-b
        c = b;
        b = a - b;
        a = c;
        if (a < b) {
            a = a + b;
            b = a - b;
            a = a - b;
        }
    }

    int d;

    d = 4;

    if (a == 6) {
        write("na barikala\n");
    }

    if (d == 4) {
        write("ok\n");
    }
}