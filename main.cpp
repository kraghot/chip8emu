#include <iostream>
#include "chip8cpu.h"

chip8cpu CPU;
SDL_Event Events;

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;
    CPU.onInit();
    for( int i = 0; i < 10000; i++)
    {
        SDL_PollEvent(&Events);
        if (Events.type == SDL_QUIT){
            break;
        }

        CPU.emulateCycle();
//        CPU.makeBMP();
        CPU.drawToWindow();
        SDL_Delay(16.6f);
    }
    return 0;
}
