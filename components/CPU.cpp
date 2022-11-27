//
// Created by viking on 11/21/22.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <thread>
#include "Input.h"
#include "CPU.h"

void CPU::loadProgram(std::string filepath) {
    std::ifstream program(filepath, std::ios::binary);

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(program), {});

    memory.write(buffer, 0x200);
    memory.printMemory();
    display.printScreenSDL();

}

void CPU::runDelayTimer() {
    while(timersRunning) {
        if (delayTimer > 0) {
            delayTimer--;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(16666)); // TODO: check if timing doesn't run into decimal problems
    }
}

void CPU::runSoundTimer() {
    while(timersRunning) {
        if (soundTimer > 0) {
            soundTimer--;
        }
        std::this_thread::sleep_for(std::chrono::microseconds (16666));
    }
}

int CPU::run() {
    // start timers
    std::thread delay(&CPU::runDelayTimer, this);
    std::thread sound(&CPU::runSoundTimer, this);

    srand((unsigned)time(0));
    unsigned char opcode;
    unsigned short instruction;

    unsigned short x, y;
    unsigned short n;
    unsigned short spriteByte;
    bool spritePixel;

    unsigned char maxOffset;

    bool running = true;

    while(running) {
        // TODO: move this
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        instruction = memory.readInstruction(pc);
        //std::cout << std::hex << instruction << std::endl;

        pc += 2;
        opcode = (unsigned char)(instruction >> 12);

        switch(opcode) {
            case 0x0:
                if ((instruction & 0x0FFF) == 0xE0) {
                    display.clearScreen();
                }
                if ((instruction & 0x0FFF) == 0xEE) {
                    pc = stack.top();
                    stack.pop();
                }
                if (instruction == 0x0000) {
                    std::cerr << "Unknown instruction abort!" << std::endl;
                    timersRunning = false;
                    delay.join();
                    sound.join();
                    return 1;
                }
                break;
            case 0x1:
                pc = instruction & 0x0FFF;
                break;
            case 0x2:
                stack.push(pc);
                pc = instruction & 0x0FFF;
                break;
            case 0x3:
                if (v[(instruction & 0x0F00) >> 8] == (instruction & 0x00FF)) {
                    pc += 2;
                }
                break;
            case 0x4:
                if (v[(instruction & 0x0F00) >> 8] != (instruction & 0x00FF)) {
                    pc += 2;
                }
                break;
            case 0x5:
                if (v[(instruction & 0x0F00) >> 8] == v[(instruction & 0x00F0) >> 4]) {
                    pc += 2;
                }
                break;
            case 0x6:
                v[(instruction & 0x0F00) >> 8] = instruction & 0x00FF;
                break;
            case 0x7:
                v[(instruction & 0x0F00) >> 8] += instruction & 0x00FF;
                break;
            case 0x8:
                switch (instruction & 0x000F) {
                    case 0:
                        v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x00F0) >> 4];
                        break;
                    case 1:
                        v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x0F00) >> 8] | v[(instruction & 0x00F0) >> 4];
                        break;
                    case 2:
                        v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x0F00) >> 8] & v[(instruction & 0x00F0) >> 4];
                        break;
                    case 3:
                        v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x0F00) >> 8] ^ v[(instruction & 0x00F0) >> 4];
                        break;
                    case 4:
                        v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x0F00) >> 8] + v[(instruction & 0x00F0) >> 4];
                        break;
                    case 5:
                        v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x0F00) >> 8] - v[(instruction & 0x00F0) >> 4];
                        break;
                    case 6:
                        // TODO: setting
                        //v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x00F0) >> 4];
                        v[0xF] = v[(instruction & 0x0F00) >> 8] & 0b0000'0001;
                        v[(instruction & 0x0F00) >> 8] >>= 1;
                        break;
                    case 7:
                        v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x00F0) >> 4] - v[(instruction & 0x0F00) >> 8];
                        break;
                    case 0xE:
                        // TODO: setting
                        //v[(instruction & 0x0F00) >> 8] = v[(instruction & 0x00F0) >> 4];
                        v[0xF] = (v[(instruction & 0x0F00) >> 8] & 0b1000'0000) >> 8;
                        v[(instruction & 0x0F00) >> 8] <<= 1;
                        break;
                }
                break;
            case 0x9:
                if (v[(instruction & 0x0F00) >> 8] != v[(instruction & 0x00F0) >> 4]) {
                    pc += 2;
                }
                break;
            case 0xA:
                i = (instruction & 0x0FFF);
                break;
            case 0xB:
                pc = (instruction & 0xFFF) + v[0];

                // TODO: Add setting
                // pc = (instruction & 0xFFF) + v[instruction & 0x0F00 >> 8];
                break;
            case 0xC:
                v[instruction & 0x0F00] = (rand() % (instruction & 0x00FF)) & (instruction & 0x00FF);
                break;
            case 0xD:
                // TODO: move to function
                x = v[(instruction & 0x0F00) >> 8] % 64;
                y = v[(instruction & 0x00F0) >> 4] % 32;
                v[0xF] = 0;
                n = instruction & 0x000F;
                for (int k = 0; k < n; k++) {
                    spriteByte = memory.readByte(i + k);
                    for (unsigned char bit = 0; bit < 8; bit++) {
                        spritePixel = (spriteByte & (0x80 >> bit)) != 0;

                        if(spritePixel && display.getPixel(x + bit, y + k)) {
                            display.setPixel(x + bit, y + k, 0b0);
                            v[0xF] = 1;
                        } else if(spritePixel && !display.getPixel(x + bit, y + k)) {
                            display.setPixel(x + bit, y + k, 0b1);
                        }
                        if (display.getCollisionRight(x + bit)){
                            bit = 8;
                        }
                    }
                    if(display.getCollisionBottom(y + k)) {
                        k = n;
                    }
                }
                display.printScreenSDL();
                break;
            case 0xE:
                if ((instruction & 0x00FF) == 0x9E) {
                    if(Input::isKeyDown(v[(instruction & 0x0F00) >> 8])) {
                        pc += 2;
                    }
                }
                if ((instruction & 0x00FF) == 0xA1) {
                    if(!Input::isKeyDown(v[(instruction & 0x0F00) >> 8])) {
                        pc += 2;
                    }
                }
                break;
            case 0xF:
                switch(instruction & 0x00FF) {
                    case 0x07:
                        v[(instruction & 0x0F00) >> 8] = delayTimer;
                        break;
                    case 0x15:
                        delayTimer = v[(instruction & 0x0F00) >> 8];
                        break;
                    case 0x18:
                        soundTimer = v[(instruction & 0x0F00) >> 8];
                        break;
                    case 0x1E:
                        i += v[(instruction & 0x0F00) >> 8];
                        // TODO: check if needed, maybe add setting
                        if (i < 0x1000) {
                            v[0xF] = 1;
                        }
                        break;
                    case 0x0A:
                        pc -= 2;
                        if(Input::getKey() < 0x10) {
                            v[(instruction & 0x0F00) >> 8] = Input::getKey();
                            pc += 2;
                        }
                        break;
                    case 0x29:
                        i = memory.getFontAddress(v[(instruction & 0x0F00) >> 8]);
                        break;
                    case 0x33:
                        memory.write({static_cast<unsigned char>(v[(instruction & 0x0F00) >> 8] / 100)}, i);
                        memory.write({static_cast<unsigned char>((v[(instruction & 0x0F00) >> 8] / 10) % 10)}, i + 1);
                        memory.write({static_cast<unsigned char>(v[(instruction & 0x0F00) >> 8] % 10)}, i + 2);
                        break;
                    case 0x55:
                        maxOffset = (instruction & 0x0F00) >> 8;
                        for(unsigned char offset = 0; offset <= maxOffset; offset++) {
                            memory.write({v[offset]}, i + offset);
                        }
                        break;
                    case 0x65:
                        maxOffset = (instruction & 0x0F00) >> 8;
                        for(unsigned char offset = 0; offset <= maxOffset; offset++) {
                            v[offset] = memory.readByte(i + offset);
                        }
                        break;
                }
                break;
            default:
                std::cerr << "Unknown instruction abort!" << std::endl;
                timersRunning = false;
                delay.join();
                sound.join();
                return 1;
        }
        std::this_thread::sleep_for(std::chrono::microseconds (3500));
    }
    timersRunning = false;
    delay.join();
    sound.join();
    return 0;
}
