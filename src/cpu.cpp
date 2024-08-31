#include "cpu.h"

#include <stdexcept>

#include "log.h"

uint8_t Cpu::readFromPc(Memory& mem) {
    return mem.readAddress(reg_.pc[0], reg_.pc[1]);
};

void Cpu::setPc(uint8_t pc_low, uint8_t pc_high) {
    reg_.pc[0] = pc_low;
    reg_.pc[1] = pc_high;
}

void Cpu::executeInstruction(Memory& memory) {
    uint8_t opcode = readFromPc(memory);
    // int cycles = 0;

    switch (opcode) {
    default:
        LOGE("Unknown opcode %x", opcode)
        throw std::invalid_argument("");
        break;
    };

    // TODO Advance PPU
    // ppu.cycle(cycles);
};
