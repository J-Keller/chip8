//
// Created by viking on 11/21/22.
//

#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H
#include <stack>
#include <string>
#include <atomic>
#include "Memory.h"
#include "Display.h"

class CPU {
public:
    void loadProgram(std::string filepath);
    int run();
protected:
private:
    void runDelayTimer();
    void runSoundTimer();


    Memory memory;
    Display display;

    unsigned int pc = 0x200;
    unsigned short i = 0;
    std::stack<unsigned short> stack;
    std::atomic<bool> timersRunning = true;
    std::atomic<unsigned char> delayTimer = 0;
    std::atomic<unsigned char> soundTimer = 0;
    unsigned char v[16] = {};
};


#endif //CHIP8_CPU_H
