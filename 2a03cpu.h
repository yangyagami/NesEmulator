/*
        7  bit  0
        ---- ----
        NVss DIZC
        |||| ||||
        |||| |||+- Carry
        |||| ||+-- Zero
        |||| |+--- Interrupt Disable
        |||| +---- Decimal
        ||++------ No CPU effect, see: the B flag
        |+-------- Overflow
        +--------- Negative
*/
#include <stdint.h>

struct CPU;

struct OPCODE {
    uint8_t len;
    uint8_t cycle;
    uint8_t flag;
    uint8_t (*addressingMode)(struct CPU *cpu, uint8_t *ram, uint8_t flag);
    void (*fun)(struct CPU *cpu, uint8_t *ram);
};

struct CPU {
    //registers
    uint8_t A;         // Accumulator
    uint8_t X;         // X
    uint8_t Y;         // Y
    uint16_t PC;       // Program Counter
    uint8_t SP;        // stack pointer
    uint8_t P;         // Status Register

    //cycle, opcodes, address
    uint8_t cycle;
    struct OPCODE opcodes[0xFF];
    uint16_t address;
};

//CPU
void CPU_init(struct CPU *cpu);
uint8_t CPU_fetch(struct CPU *cpu, uint8_t *ram);
void CPU_clock(struct CPU *cpu, uint8_t *ram);
void CPU_setCarryFlag(struct CPU *cpu);
void CPU_setZeroFlag(struct CPU *cpu);
void CPU_setInterrputFlag(struct CPU *cpu);
void CPU_setDecimalFlag(struct CPU *cpu);
void CPU_setOverflowFlag(struct CPU *cpu);
void CPU_setNegativeFlag(struct CPU *cpu);

//operator
void ADC(struct CPU *cpu, uint8_t *ram);
void STA(struct CPU *cpu, uint8_t *ram);
void LDA(struct CPU *cpu, uint8_t *ram);
void AND(struct CPU *cpu, uint8_t *ram);
void ASL(struct CPU *cpu, uint8_t *ram);

//addressing mode
//flag means if need add crossed page
uint8_t Accumulator(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t Immediate(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t Implicit(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t ZeroPage(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t ZeroPageX(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t ZeroPageY(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t Absolute(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t AbsoluteX(struct CPU *cpu, uint8_t *ram, uint8_t flag); 
uint8_t AbsoluteY(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t Indirect(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t IndirectX(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t IndirectY(struct CPU *cpu, uint8_t *ram, uint8_t flag);
uint8_t Relative(struct CPU *cpu, uint8_t *ram, uint8_t flag);              