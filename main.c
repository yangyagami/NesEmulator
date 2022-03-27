#include <stdio.h>
#include <stdlib.h>
#include "2a03cpu.h"

int main() {
    struct CPU cpu;
    uint8_t ram[2048] = {0xa9, 0x70, 0x0a};
    CPU_init(&cpu);

    while (1) {
        CPU_clock(&cpu, ram);
        //for debug
        if (cpu.PC == 3) {
            break;
        }
    }
    return 0;
}