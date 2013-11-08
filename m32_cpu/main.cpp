#include <iostream>
#include <cstring>

using namespace std;

#include "m32_cpu.h"

int main()
{
    m32_cpu c;
    c.callback_print=[](std::string str){std::cout<<"[OUT]"<<str<<"[END_OUT]"<<std::endl;};
    c.callback_log=[](std::string str){std::cout<<str;};
    c.load("data.bin");
    c.print_memory(0,0x0F);
    c.step();c.step();c.step();c.step();c.step();c.step();c.step();c.step();c.step();c.step();
    //c.run();
    return 0;
}

