#include <iostream>
#include "components/CPU.h"


// TODO: Clock Program -> Input is way to fast, instantly fills all six values
// TODO: Framed MK1 and MK2 not working -> maybe random ist bugged
// TODO: Jumping X and O -> Segfault

int main(int argc, char* argv[]) {
    CPU cpu;
    cpu.loadProgram(argv[1]); // TODO: argument validation
    return cpu.run();
}
