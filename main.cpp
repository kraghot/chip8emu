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

        if (CPU.getDrawFlag())
            CPU.drawToWindow();

        SDL_Delay(5);
    }
    return 0;
}
