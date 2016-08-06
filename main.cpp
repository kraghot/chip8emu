#include <iostream>
#include "chip8cpu.h"

chip8cpu CPU;
SDL_Event Events;

int main(int argc, char *argv[])
{
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
        SDL_Delay(1);
    }
    return 0;
}
