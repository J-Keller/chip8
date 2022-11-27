#include <iostream>
#include "components/CPU.h"


// TODO: Clock Program -> Input is way to fast, instantly fills all six values
// TODO: Framed MK1 and MK2 not working -> maybe random ist bugged
// TODO: Jumping X and O -> Segfault

int main() {
    CPU cpu;
    cpu.loadProgram("./roms/games/Nim [Carmelo Cortez, 1978].ch8");
    return cpu.run();
}
