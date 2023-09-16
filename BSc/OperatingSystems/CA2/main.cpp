#include <iostream>
#include <string>
#include <sstream>

#include "pcounter.hpp"
#include "tcounter.hpp"

int main(int argc , char *argv[])
{
    if (argc != 4) {
        std::cout<<"few args ...!"<<std::endl;
        exit(1);
    }
    
    std::stringstream st;
    std::string s; int n;
    
    st << argv[3]; st >> n;
    TotalCounter *t = new TotalCounter(argv[1], argv[2]);
    
    std::cout <<t->count(n) << std::endl;
}
