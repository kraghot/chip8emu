#include "chip8cpu.h"

chip8cpu::chip8cpu()
{

}

bool chip8cpu::onInit()
{
    srand (time(NULL));

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
        SDL_Delay(16.6f);
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

    programFile.open("../roms/PONG", std::ios::binary | std::ios::in);
    programFile.seekg(0, std::ios::end);
    fileSize = programFile.tellg();
    programFile.seekg(0, std::ios::beg);
    programFile.read( (char*) &memory[0x200], fileSize);
    programFile.close();

    return true;
}

void chip8cpu::emulateCycle()
{
    opcode = memory[programCounter] << 8 | memory[programCounter + 1];

    switch (opcode & 0xF000) {
    case 0x0000:
        switch(opcode & 0x000F)
        {
            case 0x0000: // 0x00E0: Clears the screen
            std::fill(std::begin(gfx), std::end(gfx), 0);
            programCounter += 2;
            break;

            case 0x000E: // 0x00EE: Returns from subroutine
            programCounter = stack[--stackPointer];
            break;

            default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);
        }
        break;

    case 0x1000:
        programCounter = opcode & 0x0FFF;
        break;

    case 0x2000:
      stack[stackPointer++] = programCounter + 2; // does +2 belong here
      programCounter = opcode & 0x0FFF;
    break;

    case 0x3000:
        if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        {
            programCounter += 2;
        }
        programCounter += 2;
        break;

    case 0x4000:
        if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        {
            programCounter += 2;
        }
        programCounter += 2;
        break;

    case 0x5000:
        if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4])
        {
            programCounter += 2;
        }
        programCounter += 2;
        break;

    case 0x6000:
        registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        programCounter += 2;
        break;

    case 0x7000:
        registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        programCounter += 2;
        break;

    case 0x8000:
        X8 = (opcode & 0x0F00) >> 8;
        Y8 = (opcode & 0x00F0) >> 4;

        switch (opcode & 0x000F) {
        case 0:
            registers[X8] = registers[Y8];
            break;
        case 1:
            registers[X8] |= registers[Y8];
            break;
        case 2:
            registers[X8] &= registers[Y8];
            break;
        case 3:
            registers[X8] ^= registers[Y8];
            break;
        case 4:
            if ((uint16_t)(registers[Y8] + registers[X8]) > 255)
                registers[0xF] = 1;
            else
                registers[0xF] = 0;

            registers[X8] += registers[Y8];
            break;
        case 5:
            if (registers[X8] < registers[Y8])
                registers[0xF] = 1;
            else
                registers[0xF] = 0;

            registers[X8] -= registers[Y8];
            break;
        case 6:
            registers[0xF] = registers[X8] & 0x000F;
            registers[X8] = registers[X8] >> 1;
            break;
        case 7:
            if (registers[X8] > registers[Y8])
                registers[0xF] = 1;
            else
                registers[0xF] = 0;
            registers[X8] = registers[Y8] - registers[X8];
            break;
        case 14:
            registers[0xF] = registers[X8] >> 7;
            registers[X8] = registers[X8] << 1;
            break;

        default:
            printf ("Unknown opcode [0x8XY?]: 0x%X\n", opcode);
            break;
        }
        programCounter += 2;
        break;

    case 0x9000:
        if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4])
        {
            programCounter += 2;
        }
        programCounter += 2;
        break;

    case 0xA000: // ANNN set adress to NNN
        indexReg = opcode & 0x0FFF;
        programCounter += 2;
        break;

    case 0xB000:
        programCounter += (opcode & 0x0FFF) + registers[0];
        break;

    case 0xC000:
        randomNumber = rand() % 256;
        registers[(opcode & 0x0F00) >> 8] = randomNumber & (opcode & 0x00FF);
        programCounter += 2;
        break;

    case 0xD000:
    {
      unsigned short x = registers[(opcode & 0x0F00) >> 8];
      unsigned short y = registers[(opcode & 0x00F0) >> 4];
      unsigned short height = opcode & 0x000F;
      unsigned short pixel;

      registers[0xF] = 0;
      for (int yline = 0; yline < height; yline++)
      {
        pixel = memory[indexReg + yline];
        for(int xline = 0; xline < 8; xline++)
        {
          if((pixel & (0x80 >> xline)) != 0)
          {
            if(gfx[(x + xline + ((y + yline) * 64))] == 1)
              registers[0xF] = 1;
            gfx[x + xline + ((y + yline) * 64)] ^= 1;
          }
        }
      }

      drawFlag = true;
      programCounter += 2;
    }
    break;

    case 0xE000:
        keyPressedFlag = key[registers[(opcode & 0x0F00) >> 8]];
        switch (opcode & 0x00FF){
            case 0x009E:
                if(keyPressedFlag) programCounter += 2;
                break;
            case 0x00A1:
                if(!keyPressedFlag) programCounter += 2;
                break;
            default:
                printf("Unknown opcode 0x%X\n", opcode);
                break;
        }
        programCounter += 2;
        break;

    case 0xF000:
        X8 = (opcode & 0x0F00) >> 8;

        switch (opcode & 0x00FF) {

        case 0x0007:
            registers[X8] = delayTimer;
            break;

        case 0x000A:
            //implement
            break;

        case 0x0015:
            delayTimer = registers[X8];
            break;

        case 0x0018:
            soundTimer = registers[X8];
            break;

        case 0x001E:
            indexReg += registers[X8];
            break;

        case 0x0029:
            // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
            indexReg = registers[X8] * 4;
            break;

        case 0x0033:
            memory[indexReg]     = registers[(opcode & 0x0F00) >> 8] / 100;
            memory[indexReg + 1] = (registers[(opcode & 0x0F00) >> 8] / 10) % 10;
            memory[indexReg + 2] = (registers[(opcode & 0x0F00) >> 8] % 100);
            break;

        case 0x0055:
            for (int i = 0; i <= X8; i++)
            {
                memory[indexReg++] = registers[i];
            }
            break;

        case 0x0065:
            for (int i = 0; i <= X8; i++)
            {
                registers[i] = memory[indexReg + i];
            }
            break;

        default:
            printf("Unknown opcode 0x%X\n", opcode);
            break;
        }
        programCounter += 2;
        break;

    default:
        printf("Unknown opcode 0x%X\n", opcode);
        programCounter += 2;
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

void chip8cpu::makeBMP() {
    const int w = 64;
    const int h = 32;

    image = new bitmap_image(64, 32);

    image->set_all_channels(0, 0 ,0);

    for (int j = 0; j < h; j++){
        for (int i = 0; i < w; i++){
            if(gfx[i + j * h])
                image->set_pixel(i, j, 255, 255, 255);
        }
    }
}

void chip8cpu::drawToWindow()
{
    const int w = 64;
    const int h = 32;
    const int scalingFactor = 8;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rect;
    for (int j = 0; j < h; j++){
        for (int i = 0; i < w; i++){
            if(gfx[i + j * 64])
            {
                rect.w = scalingFactor;
                rect.h = scalingFactor;
                rect.x = i*scalingFactor;
                rect.y = j*scalingFactor;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
