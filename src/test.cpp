#include "Inspire.hpp"
#include <iostream>
#include <string.h>
int main(int argc, char const *argv[])
{
    Inspire inspire;
    if(inspire.InitCan()!=0 )
    {
        std::cout << "It is error when init can" << std::endl;
        return 1;
    }
    if(inspire.WriteCan(1486,100,1)!=0)
    {
        std::cout<<"It is error when write"<<std::endl;
    }
    
    return 0;
}
