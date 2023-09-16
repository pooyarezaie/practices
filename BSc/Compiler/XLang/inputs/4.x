void main()
{
   [int, int, int] b = [2, 1, 4];

   int c = 0;
   foreach(int a in b) {
        c = c + a;
   }

   if (c == 7) {
        write("afarin\n");
   }

   list(4) of string(5) strs = ["sala\n", "salb\n", "salc\n", "sald\n"];

   foreach(string(5) str in strs)
        write(str);


   write("test2: \n");


   while(c > 0) {
        if (c == 4 || c == 2)
            write("kam1\n");
        else if (c == 3 || c == 6)
            write("kam2\n");
        else
            write("kam3\n");
        c = c - 1;
   }



}