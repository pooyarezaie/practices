void main() {
    [first : int, second : int, third : int] a;
    a.second = 7;
    int c = 10;
    int temp = a.second + c;
    if (temp == 16) {
        write("are\n");
    }

    a#2 = 2;

    a#2 = a#2 + a#1;

    int d = a#2;

    if (d == 9) {
        write(":)");
    }
}