//
// Created by viking on 11/21/22.
//

#include <iostream>
#include <ios>
#include <iomanip>
#include "Memory.h"


Memory::Memory() {
    initializeMemory();
}

void Memory::initializeMemory() {
    std::copy(std::begin(font), std::end(font), std::begin(ram) + 0x50);
}

int Memory::write(std::vector<unsigned char> buffer, unsigned short address) {
    std::copy(buffer.begin(), buffer.end(), std::begin(ram) + address);

    return 0; // if successful
}

unsigned short Memory::readInstruction(unsigned short pc) {
    return (ram[pc] << 8) + ram[pc + 1];
}

unsigned char Memory::readByte(unsigned short address) {
    return ram[address];
}

unsigned short Memory::getFontAddress(unsigned char character) {
    return 0x50 + (character * 5);
}

// Debug Stuff
unsigned char *Memory::readMemoryContent() {
    return ram;
}

void Memory::printMemory() {
    for(unsigned short byte = 0; byte < 4096; byte += 2) {
        std::cout << std::hex << byte << ": " << std::setw(2) << std::setfill('0') << std::hex << (unsigned short)ram[byte] << std::setw(2) << std::setfill('0') << (unsigned short)ram[byte + 1] << std::endl;
    }
}
