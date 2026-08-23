#include "cpu.h"

#include <stdexcept>
#include <tuple>

#include "system.h"

#include "log.h"

Cpu::Cpu() {
    setPc(0xFC, 0xFF);
    reg_.sp = 0xFD;

    reg_.a = 0;
    reg_.x = 0;
    reg_.y = 0;

    reg_.p = 0;
    setStatusFlag(StatusFlag::INTERRUPT, true);
    setStatusFlag(StatusFlag::UNUSED, true);
}

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

int Cpu::executeInstruction(ParsedInstruction current_instruction) {
    incPc(current_instruction.num_args_);

    try {
        switch (current_instruction.type_) {
        #include "instructions.h"
        default:
            LOGE("Unknown opcode %x", current_instruction.opcode_)
            throw std::invalid_argument("");
            break;
        };

        return current_instruction.cycles_;
    } catch (std::out_of_range& e) {
        LOGE("Unknown opcode %x", current_instruction.opcode_);
        throw e;
    }
};

Cpu::Registers Cpu::getRegisters() {
    return reg_;
}
