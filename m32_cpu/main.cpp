#include <iostream>
#include <cstring>

using namespace std;

#include "m32_cpu.h"

int main()
{
    m32_cpu c;
    c.print_callback=[](std::string str){std::cout<<str;};
    c.load("data.bin");
    c.print_memory(0,0x0F);
    c.step();c.step();c.step();c.step();c.step();c.step();c.step();c.step();c.step();c.step();
    //c.run();
    return 0;
}

