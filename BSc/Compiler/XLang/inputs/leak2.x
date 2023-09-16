//leak - 1
void main(){
    int a;
    a=2;
    {
        int b;
        b=3;
    }
    int c;
    c=4;
}
