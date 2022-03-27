#include <stdio.h>
#include <stdlib.h>
#include "2a03cpu.h"

static void debug(struct CPU *cpu, uint8_t *ram) {
    printf("register:\n");
    printf("A:0x%x X:0x%x Y:0x%x\n", cpu->A, cpu->X, cpu->Y);
    printf("PC:0x%x SP:0x%x\n", cpu->PC, cpu->SP);
    printf("\n");
    printf("cycle: %d\n", cpu->cycle);
    printf("address: 0x%x\n", cpu->address);
    printf("\n");
    printf("NVssDIZC\n");
    char s[8] = { 0 };
    itoa(cpu->P, s, 2);
    printf("%s", s);
    printf("\n");
    for (int i = 0; i < 128; i++) {
        printf("0x%x: ", 0x0000 + i * 16);
        for (int j = 0; j < 16; j++) {
            printf("0x%x ", ram[i * 16 + j]);
        }
        printf("\n");
    }
}

void CPU_init(struct CPU *cpu) {
/*
        ADC (ADd with Carry)
        Affects Flags: N V Z C

        MODE           SYNTAX       HEX LEN TIM
        Immediate     ADC #$44      $69  2   2
        Zero Page     ADC $44       $65  2   3
        Zero Page,X   ADC $44,X     $75  2   4
        Absolute      ADC $4400     $6D  3   4
        Absolute,X    ADC $4400,X   $7D  3   4+
        Absolute,Y    ADC $4400,Y   $79  3   4+
        Indirect,X    ADC ($44,X)   $61  2   6
        Indirect,Y    ADC ($44),Y   $71  2   5+

        + add 1 cycle if page boundary crossed
*/
    cpu->opcodes[0x69] = (struct OPCODE){ 2, 2, 0, Immediate, ADC };
    cpu->opcodes[0x65] = (struct OPCODE){ 2, 3, 0, ZeroPage, ADC };
    cpu->opcodes[0x75] = (struct OPCODE){ 2, 4, 0, ZeroPageX, ADC };
    cpu->opcodes[0x6D] = (struct OPCODE){ 3, 4, 0, Absolute, ADC };
    cpu->opcodes[0x7D] = (struct OPCODE){ 3, 4, 1, AbsoluteX, ADC };
    cpu->opcodes[0x79] = (struct OPCODE){ 3, 4, 1, AbsoluteY, ADC };
    cpu->opcodes[0x61] = (struct OPCODE){ 2, 6, 0, IndirectX, ADC };
    cpu->opcodes[0x71] = (struct OPCODE){ 2, 5, 1, IndirectY, ADC };
    
/*
    LDA (LoaD Accumulator)
    Affects Flags: N Z

    MODE           SYNTAX       HEX LEN TIM
    Immediate     LDA #$44      $A9  2   2
    Zero Page     LDA $44       $A5  2   3
    Zero Page,X   LDA $44,X     $B5  2   4
    Absolute      LDA $4400     $AD  3   4
    Absolute,X    LDA $4400,X   $BD  3   4+
    Absolute,Y    LDA $4400,Y   $B9  3   4+
    Indirect,X    LDA ($44,X)   $A1  2   6
    Indirect,Y    LDA ($44),Y   $B1  2   5+

    + add 1 cycle if page boundary crossed
*/
    cpu->opcodes[0xA9] = (struct OPCODE){ 2, 2, 0, Immediate, LDA };
    cpu->opcodes[0xA5] = (struct OPCODE){ 2, 3, 0, ZeroPage, LDA };
    cpu->opcodes[0xB5] = (struct OPCODE){ 2, 4, 0, ZeroPageX, LDA };
    cpu->opcodes[0xAD] = (struct OPCODE){ 3, 4, 0, Absolute, LDA };
    cpu->opcodes[0xBD] = (struct OPCODE){ 3, 4, 1, AbsoluteX, LDA };
    cpu->opcodes[0xB9] = (struct OPCODE){ 3, 4, 1, AbsoluteY, LDA };
    cpu->opcodes[0xA1] = (struct OPCODE){ 2, 6, 0, IndirectX, LDA };
    cpu->opcodes[0xB1] = (struct OPCODE){ 2, 5, 1, IndirectY, LDA };
/*
    STA (STore Accumulator)
    Affects Flags: none

    MODE           SYNTAX       HEX LEN TIM
    Zero Page     STA $44       $85  2   3
    Zero Page,X   STA $44,X     $95  2   4
    Absolute      STA $4400     $8D  3   4
    Absolute,X    STA $4400,X   $9D  3   5
    Absolute,Y    STA $4400,Y   $99  3   5
    Indirect,X    STA ($44,X)   $81  2   6
    Indirect,Y    STA ($44),Y   $91  2   6
*/
    cpu->opcodes[0x85] = (struct OPCODE){ 2, 3, 0, ZeroPage, STA };
    cpu->opcodes[0x95] = (struct OPCODE){ 2, 4, 0, ZeroPageX, STA };
    cpu->opcodes[0x8D] = (struct OPCODE){ 3, 4, 0, Absolute, STA };
    cpu->opcodes[0x9D] = (struct OPCODE){ 3, 5, 0, AbsoluteX, STA };
    cpu->opcodes[0x99] = (struct OPCODE){ 3, 5, 0, AbsoluteY, STA };
    cpu->opcodes[0x81] = (struct OPCODE){ 2, 6, 0, IndirectX, STA };
    cpu->opcodes[0x91] = (struct OPCODE){ 2, 6, 0, IndirectY, STA };
/*
    AND (bitwise AND with accumulator)
    Affects Flags: N Z

    MODE           SYNTAX       HEX LEN TIM
    Immediate     AND #$44      $29  2   2
    Zero Page     AND $44       $25  2   3
    Zero Page,X   AND $44,X     $35  2   4
    Absolute      AND $4400     $2D  3   4
    Absolute,X    AND $4400,X   $3D  3   4+
    Absolute,Y    AND $4400,Y   $39  3   4+
    Indirect,X    AND ($44,X)   $21  2   6
    Indirect,Y    AND ($44),Y   $31  2   5+

    + add 1 cycle if page boundary crossed
*/
    cpu->opcodes[0x29] = (struct OPCODE){ 2, 2, 0, Immediate, AND};
    cpu->opcodes[0x25] = (struct OPCODE){ 2, 3, 0, ZeroPage, AND };
    cpu->opcodes[0x35] = (struct OPCODE){ 2, 4, 0, ZeroPageX, AND};
    cpu->opcodes[0x2D] = (struct OPCODE){ 3, 4, 0, Absolute, AND };
    cpu->opcodes[0x3D] = (struct OPCODE){ 3, 4, 1, AbsoluteX, AND };
    cpu->opcodes[0x39] = (struct OPCODE){ 3, 4, 1, AbsoluteY, AND };
    cpu->opcodes[0x21] = (struct OPCODE){ 2, 6, 0, IndirectX, AND };
    cpu->opcodes[0x31] = (struct OPCODE){ 2, 5, 1, IndirectY, AND };
/*
    ASL (Arithmetic Shift Left)
    Affects Flags: N Z C

    MODE           SYNTAX       HEX LEN TIM
    Accumulator   ASL A         $0A  1   2
    Zero Page     ASL $44       $06  2   5
    Zero Page,X   ASL $44,X     $16  2   6
    Absolute      ASL $4400     $0E  3   6
    Absolute,X    ASL $4400,X   $1E  3   7

    ASL shifts all bits left one position. 0 is shifted into bit 
    0 and the original bit 7 is shifted into the Carry.    
*/
    cpu->opcodes[0x0A] = (struct OPCODE){ 1, 2, 0, Accumulator, ASL};
    cpu->opcodes[0x06] = (struct OPCODE){ 2, 5, 0, ZeroPage, ASL};
    cpu->opcodes[0x16] = (struct OPCODE){ 2, 6, 0, ZeroPageX, ASL};
    cpu->opcodes[0x0E] = (struct OPCODE){ 3, 6, 0, Absolute, ASL};
    cpu->opcodes[0x1E] = (struct OPCODE){ 3, 7, 0, AbsoluteX, ASL };

    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->PC = 0;
    cpu->SP = 0;
    cpu->P = 0;
    
    cpu->cycle = 0;
}

void CPU_setCarryFlag(struct CPU *cpu) {
    //carry
    cpu->P |= 0x1;
}

void CPU_setZeroFlag(struct CPU *cpu) {
    cpu->P |= 0x02;
}

void CPU_setInterrputFlag(struct CPU *cpu) {

}

void CPU_setDecimalFlag(struct CPU *cpu) {

}

void CPU_setOverflowFlag(struct CPU *cpu) {
    cpu->P |= 0x40;
}

void CPU_setNegativeFlag(struct CPU *cpu) {
    cpu->P |= 0x80;
}

uint8_t CPU_fetch(struct CPU *cpu, uint8_t *ram) {
    return ram[cpu->PC];
}

void CPU_clock(struct CPU *cpu, uint8_t *ram) {
    if (cpu->cycle <= 0) {
        uint8_t opcode = CPU_fetch(cpu, ram);
        cpu->cycle = cpu->opcodes[opcode].cycle;
        uint8_t flag = cpu->opcodes[opcode].flag;
        cpu->cycle += cpu->opcodes[opcode].addressingMode(cpu, ram, flag);
        cpu->opcodes[opcode].fun(cpu, ram);
        debug(cpu, ram);
        cpu->PC += cpu->opcodes[opcode].len;
    }
    (cpu->cycle)--;
}

//operator
void ADC(struct CPU *cpu, uint8_t *ram) {
    uint8_t result = cpu->A + ram[cpu->address];
    //carry
    if (cpu->A >= 0xFF - ram[cpu->address]) {
        CPU_setCarryFlag(cpu);
    }
    //zero
    if (result == 0) {
        CPU_setZeroFlag(cpu);
    }
    //overflow
    if ((cpu->A) >> 7 == ram[cpu->address] >> 7) {
        if ((cpu->A) >> 7 != (result >> 7)) {
            CPU_setOverflowFlag(cpu);
        }
    }
    //negative
    if ((cpu->A) >> 7 != ram[cpu->address] >> 7) {
        if ((result >> 7) == 1) {
            CPU_setNegativeFlag(cpu);
        }
    }

    cpu->A = result;
}

void STA(struct CPU *cpu, uint8_t *ram) {
    ram[cpu->address] = cpu->A;
}

void LDA(struct CPU *cpu, uint8_t *ram) {
    cpu->A = ram[cpu->address];
    //Negative
    if ((cpu->A) >> 7 == 1) {
        CPU_setNegativeFlag(cpu);
    }
    //Zero
    if ((cpu->A) == 0) {
        CPU_setZeroFlag(cpu);
    }
}

void AND(struct CPU *cpu, uint8_t *ram) {
    cpu->A = cpu->A & ram[cpu->address];
    //Negative
    if ((cpu->A) >> 7 == 1) {
        CPU_setNegativeFlag(cpu);
    }
    //Zero
    if ((cpu->A) == 0) {
        CPU_setZeroFlag(cpu);
    }
}

void ASL(struct CPU *cpu, uint8_t *ram) {
    uint8_t before;
    if (cpu->address == cpu->PC) {
        before = cpu->A;
        cpu->A = (cpu->A) << 1;
    } else {
        before = ram[cpu->address];
        cpu->A = ram[cpu->address] << 1;  
    }

    //Zero
    if ((cpu->A) == 0) {
        CPU_setZeroFlag(cpu);
    }
    //Negative
    if ((cpu->A) >> 7 == 1) {
        CPU_setNegativeFlag(cpu);
    }
    //Carry
    if (before >> 7 == 1) {
        CPU_setCarryFlag(cpu);
    }
}

//addressing mode
uint8_t Immediate(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    cpu->address = cpu->PC + 1;
    return 0;
}

uint8_t Implicit(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    return 0;
}

uint8_t Accumulator(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    cpu->address = cpu->PC;
    return 0;
}

uint8_t ZeroPage(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    cpu->address = ram[cpu->PC + 1] & 0x00FF;
    return 0;
}

uint8_t ZeroPageX(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    cpu->address = (ram[cpu->PC + 1] + cpu->X) & 0x00FF;
    return 0;
}

uint8_t ZeroPageY(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    cpu->address = (ram[cpu->PC + 1] + cpu->Y) & 0x00FF;
    return 0;
}

uint8_t Absolute(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    uint8_t lo = ram[cpu->PC + 1];
    uint8_t hi = ram[cpu->PC + 2];

    cpu->address = (hi << 8) | lo;
    return 0;
}

uint8_t AbsoluteX(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    uint8_t lo = ram[cpu->PC + 1];
    uint8_t hi = ram[cpu->PC + 2];

    cpu->address = (hi << 8) | lo;
    cpu->address += cpu->X;

    if (flag && (cpu->address >> 8) > hi) {
        return 1;
    }

    return 0;
}

uint8_t AbsoluteY(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    uint8_t lo = ram[cpu->PC + 1];
    uint8_t hi = ram[cpu->PC + 2];

    cpu->address = (hi << 8) | lo;

    cpu->address += cpu->Y;

    if (flag && (cpu->address >> 8) > hi) {
        return 1;
    }

    return 0;
}

uint8_t Indirect(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    uint8_t lo = ram[cpu->PC + 1];
    uint8_t hi = ram[cpu->PC + 2];
    cpu->address = ((hi << 8) + lo);
    lo = ram[cpu->address];
    hi = ram[cpu->address + 1];
    cpu->address = ((hi << 8) + lo);
}

uint8_t IndirectX(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    uint8_t indirectAddress = (ram[cpu->PC + 1] + cpu->X) & 0x00FF;
    uint8_t lo = ram[indirectAddress];
    uint8_t hi = ram[indirectAddress + 1];
    cpu->address = (hi << 8) + lo;
    return 0;
}

uint8_t IndirectY(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    uint8_t lo = ram[cpu->PC + 1];
    uint8_t hi = ram[cpu->PC + 2];
    cpu->address = ((hi << 8) + lo);
    hi = cpu->address >> 8;
    cpu->address += cpu->Y;
    if (flag && (cpu->address >> 8) > hi) {
        return 1;
    }
    return 0;
}

uint8_t Relative(struct CPU *cpu, uint8_t *ram, uint8_t flag) {
    cpu->address = ram[cpu->PC + 1] + cpu->PC;
    return 0;
}