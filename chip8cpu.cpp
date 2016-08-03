#include "chip8cpu.h"

chip8cpu::chip8cpu()
{

}

bool chip8cpu::onInit()
{

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        return false;
    }

    screen = SDL_CreateWindow("CHIP8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(screen == nullptr)
    {
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(renderer == nullptr)
    {
        SDL_DestroyWindow(screen);
        SDL_Quit();
        return false;
    }

    std::string imagePath = "../test.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(screen);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(screen);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    for (int i = 0; i < 3; ++i){
        //First clear the renderer
        SDL_RenderClear(renderer);
        //Draw the texture
        SDL_RenderCopy(renderer, tex, NULL, NULL);
        //Update the screen
        SDL_RenderPresent(renderer);
        //Take a quick break after all that hard work
        SDL_Delay(1000);
    }

    programCounter  = 0x200;    // Uvijek pocinje na 200
    opcode          = 0;        // Clear opcode
    indexReg        = 0;        // Clear index reg
    stackPointer    = 0;        // Reset stack pointer

    // Clear display
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    // Load fontset
    for(int i = 0; i < 80; ++i)
      memory[i] = chip8_fontset[i];

    programFile.open("../testfile.dat", std::ios::binary | std::ios::in);
    programFile.read( (char*) &memory[512], programFile.tellg());


    return true;
}

void chip8cpu::emulateCycle()
{
    opcode = memory[programCounter] << 8 | memory[programCounter + 1];

    switch (opcode & 0xF000) {
    case 0xA000: // ANNN set adress to NNN
        indexReg = opcode & 0x0FFF;
        programCounter += 2;
        break;
    default:
        printf("Unknown opcode 0x%X\n", opcode);
        break;
    }

    if(delayTimer > 0)
    {
        --delayTimer;
    }

    if(soundTimer > 0)
    {
        if(soundTimer == 1) printf("BEEEP!\n");
        --soundTimer;
    }
}

//  0NNN	Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
//  00E0	Clears the screen.
//  00EE	Returns from a subroutine.
//  1NNN	Jumps to address NNN.
//  2NNN	Calls subroutine at NNN.
//  3XNN	Skips the next instruction if VX equals NN.
//  4XNN	Skips the next instruction if VX doesn't equal NN.
//  5XY0	Skips the next instruction if VX equals VY.
//  6XNN	Sets VX to NN.
//  7XNN	Adds NN to VX.
//  8XY0	Sets VX to the value of VY.
//  8XY1	Sets VX to VX or VY.
//  8XY2	Sets VX to VX and VY.
//  8XY3	Sets VX to VX xor VY.
//  8XY4	Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
//  8XY5	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
//  8XY6	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.[2]
//  8XY7	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
//  8XYE	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.[2]
//  9XY0	Skips the next instruction if VX doesn't equal VY.
//  ANNN	Sets I to the address NNN.
//  BNNN	Jumps to the address NNN plus V0.
//  CXNN	Sets VX to the result of a bitwise and operation on a random number and NN.
//  DXYN	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
//  EX9E	Skips the next instruction if the key stored in VX is pressed.
//  EXA1	Skips the next instruction if the key stored in VX isn't pressed.
//  FX07	Sets VX to the value of the delay timer.
//  FX0A	A key press is awaited, and then stored in VX.
//  FX15	Sets the delay timer to VX.
//  FX18	Sets the sound timer to VX.
//  FX1E	Adds VX to I.[3]
//  FX29	Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
//  FX33	Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
//  FX55	Stores V0 to VX (including VX) in memory starting at address I.[4]
//  FX65	Fills V0 to VX (including VX) with values from memory starting at address I.[4]0
