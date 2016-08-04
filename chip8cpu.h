#ifndef CHIP8CPU_H
#define CHIP8CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <SDL2/SDL.h>
#include "bitmap_image.hpp"



class chip8cpu
{
public:
    chip8cpu();
    bool onInit();
    void emulateCycle();
    void makeBMP();
    void drawToWindow();
private:
    bool            Running;
    SDL_Window*     screen;
    SDL_Renderer*   renderer;

    uint16_t        opcode;
    uint8_t         memory[4096]    =   {0};
    uint8_t         registers[16]   =   {0};
    uint16_t        indexReg;
    uint16_t        programCounter;
    // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    // 0x200-0xFFF - Program ROM and work RAM
    uint8_t         gfx[64*32]      =   {0};
    uint8_t         delayTimer;
    uint8_t         soundTimer;
    uint16_t        stack[16];
    uint16_t        stackPointer;
    uint8_t         key[16]         =   {0};
    bool            drawFlag = false;

    bitmap_image* image;

    uint8_t chip8_fontset[80] =
    {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    std::ifstream programFile;
    long unsigned int fileSize;

    uint8_t X8;
    uint8_t Y8;
    uint8_t randomNumber;
    bool    keyPressedFlag;
};

#endif // CHIP8CPU_H
