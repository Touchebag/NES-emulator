#include "cpu.h"

#include <stdexcept>

#include "log.h"

void Cpu::setStatusFlag(StatusFlag bit, bool value) {
    if (value) {
        reg_.p |= static_cast<uint8_t>(bit);
    } else {
        reg_.p &= ~static_cast<uint8_t>(bit);
    }
}

void Cpu::setNegativeFlag(uint8_t value) {
    setStatusFlag(StatusFlag::NEGATIVE, (value >> 7) & 1);
}

void Cpu::setZeroFlag(uint8_t value) {
    setStatusFlag(StatusFlag::ZERO, (value == 0));
}

uint8_t Cpu::readFromPc(Memory& mem) {
    return mem.readAddress(reg_.pc[0], reg_.pc[1]);
};

void Cpu::incPc(int n) {
    for (int i = 0; i < n; i++) {
        if (reg_.pc[0] == 0xFF) {
            reg_.pc[1] = (reg_.pc[1] + 1) % 256;
        };
        reg_.pc[0] = (reg_.pc[0] + 1) % 256;
    };
}

void Cpu::setPc(uint8_t pc_low, uint8_t pc_high) {
    reg_.pc[0] = pc_low;
    reg_.pc[1] = pc_high;
}

void Cpu::executeInstruction(Memory& memory) {
    uint8_t opcode = readFromPc(memory);
    int cycles = 0;

    switch (opcode) {
    #include "interpreter/control.h"
    #include "interpreter/alu.h"
    #include "interpreter/rmw.h"
    default:
        LOGE("Unknown opcode %x", opcode)
        throw std::invalid_argument("");
        break;
    };

    // TODO Advance PPU
    // ppu.cycle(cycles);
};

Cpu::Registers Cpu::getRegisters() {
    return reg_;
}
