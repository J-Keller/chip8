#include <iostream>
#include "components/CPU.h"


// TODO: Clock Program -> Input is way to fast, instantly fills all six values
// TODO: Jumping X and O -> Segfault


// TODO: Debug add step mode
int main(int argc, char* argv[]) {
    CPU cpu;
    cpu.loadProgram(argv[1]); // TODO: argument validation
    return cpu.run();
}
