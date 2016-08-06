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



class chip8cpu
{
public:
    chip8cpu();
    bool onInit();
    void emulateCycle();
    void drawToWindow();
    bool getDrawFlag();
    void pollKeys();

private:
    bool            Running;
    SDL_Window*     screen;
    SDL_Renderer*   renderer;

    uint16_t        opcode;
    uint8_t         memory[4096]    =   {0};
    uint8_t         registers[16]   =   {0};
    uint16_t        indexReg;
    uint16_t        programCounter;
    uint8_t         gfx[64*32]      =   {0};
    uint8_t         delayTimer;
    uint8_t         soundTimer;
    uint16_t        stack[16];
    uint16_t        stackPointer;
    uint8_t         key[16]         =   {0};
    bool            drawFlag = true;

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
    int     sum;

};

#endif // CHIP8CPU_H
