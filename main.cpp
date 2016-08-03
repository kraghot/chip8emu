#include <iostream>
#include "chip8cpu.h"

chip8cpu CPU;

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;
    CPU.onInit();
    return 0;
}
