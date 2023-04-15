#include <iostream>
#include "SynaesthetiQ-LED-Library/src/SynaesthetiQ/synaesthetiq.hpp"


int main()
{
    std::cout << "Turning off LEDs..." << std::endl;
    SynaesthetiQ* obj = new SynaesthetiQ();
    delete obj;
    std::cout << "Exiting" << std::endl;
    return 0;
}
