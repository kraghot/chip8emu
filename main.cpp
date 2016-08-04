#include <iostream>
#include "chip8cpu.h"

chip8cpu CPU;
SDL_Event Events;

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;
    CPU.onInit();
    while(1)
    {
        SDL_PollEvent(&Events);
        if (Events.type == SDL_QUIT){
            break;
        }

        CPU.emulateCycle();
//        CPU.makeBMP();
        CPU.drawToWindow();
        SDL_Delay(1.0f/500.0f /1000.0f);
    }
    return 0;
}
