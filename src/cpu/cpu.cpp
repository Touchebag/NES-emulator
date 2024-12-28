#include "cpu.h"

#include <stdexcept>
#include <tuple>

#include "system.h"

#include "log.h"

namespace {

std::tuple<uint8_t, uint8_t, int> relativeJump(uint8_t lo, uint8_t hi, uint8_t val) {
    // Always add at least one cycle
    int cycles = 1;

    uint8_t new_lo = lo + val;
    // If relative is negative
    if (val & 128) {
        if (new_lo > lo) {
            // Underflow, carry to hi;
            hi--;
            cycles = 2;
        }
    } else if (new_lo < lo) {
        // Overflow, carry to hi;
        hi++;
        cycles = 2;
    }

    return {new_lo, hi, cycles};
}

std::pair<uint8_t, bool> absoluteX(uint8_t lo, uint8_t hi, uint8_t x) {
    uint8_t new_lo = lo + x;
    bool page_cross = false;

    if (new_lo < lo) {
        // Overflow, carry to hi
        hi++;
        page_cross = true;
    }

    uint8_t val = System::get<Memory>().readAddress(new_lo, hi);

    return {val, page_cross};
}

std::pair<uint8_t, bool> indirectX(uint8_t address, uint8_t x) {
    auto& mem = System::get<Memory>();
    uint8_t lo = mem.readAddress(address + x, 0x00);
    uint8_t hi = mem.readAddress(address + x + 0x01, 0x00);

    uint8_t val = mem.readAddress(lo, hi);

    return {val, false};
}

} // namespace

bool Cpu::getStatusFlag(Cpu::StatusFlag flag) {
    return reg_.p & static_cast<uint8_t>(flag);
}

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

uint8_t Cpu::readFromPc() {
    return System::get<Memory>().readAddress(reg_.pc[0], reg_.pc[1]);
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

void Cpu::pushStack(uint8_t value) {
    if (reg_.sp == 0x00) {
        LOGW("Stack overflow");
    }
    System::get<Memory>().writeAddress(reg_.sp--, 0x01, value);
}

uint8_t Cpu::popStack() {
    if (reg_.sp == 0xFF) {
        LOGW("Stack underflow");
    }
    return System::get<Memory>().readAddress(++reg_.sp, 0x01);
}

int Cpu::executeInstruction() {
    auto& memory = System::get<Memory>();
    uint8_t opcode = readFromPc();
    int cycles = 0;

    switch (opcode) {
    #include "control.h"
    #include "alu.h"
    #include "rmw.h"
    default:
        LOGE("Unknown opcode %x", opcode)
        throw std::invalid_argument("");
        break;
    };

    return cycles;
};

Cpu::Registers Cpu::getRegisters() {
    return reg_;
}
