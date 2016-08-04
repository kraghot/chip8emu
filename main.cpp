#include <iostream>
#include "chip8cpu.h"

chip8cpu CPU;

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;
    CPU.onInit();
    for( int i = 0; i < 50; i++)
    {
        CPU.emulateCycle();
        CPU.makeBMP();
        CPU.drawToWindow();
        SDL_Delay(1000);
    }
    return 0;
}
