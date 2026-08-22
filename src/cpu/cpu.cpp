#include "cpu.h"

#include <stdexcept>
#include <tuple>

#include "system.h"

#include "log.h"

#define READ_ARGUMENT() readArgument(current_instruction_data, cycles)
#define WRITE_ARGUMENT(x) writeArgument(current_instruction_data, cycles, x)

namespace {

std::tuple<uint8_t, uint8_t, bool> calculateRelativeJump(uint8_t lo, uint8_t hi, uint8_t val) {
    bool page_cross = false;

    uint8_t new_lo = lo + val;
    // If relative is negative
    if (val & 128) {
        if (new_lo > lo) {
            // Underflow, carry to hi;
            hi--;
            page_cross = true;
        }
    } else if (new_lo < lo) {
        // Overflow, carry to hi;
        hi++;
        page_cross = true;
    }

    return {new_lo, hi, page_cross};
}

} // namespace

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

int Cpu::executeInstruction() {
    auto current_instruction = ParsedInstruction(reg_.pc[0], reg_.pc[1]);
    current_instruction.print(0);

    incPc(current_instruction.num_args_);

    try {
        switch (current_instruction.type_) {
        #include "control.h"
        #include "alu.h"
        #include "rmw.h"
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

uint8_t Cpu::readArgument(const InstructionData& instruction_data, unsigned int& cycles) {
    uint8_t retval = 0;
    incPc(1); // Skip instruction byte

    switch (instruction_data.addr_mode) {
        case AddressingMode::RELATIVE:
            [[fallthrough]]; // Argument is treated identically
        case AddressingMode::IMMEDIATE:
            retval = readFromPc();
            incPc(1);

            break;
        case AddressingMode::ABSOLUTE_X: {
            uint8_t lo = readFromPc();
            incPc(1);
            uint8_t hi = readFromPc();
            incPc(1);
            uint8_t x = reg_.x;

            uint8_t new_lo = lo + x;

            if (new_lo < lo) {
                // Overflow, carry to hi
                hi++;
                // Page cross, extra cycle
                cycles++;
            }

            retval = System::get<Memory>().readAddress(new_lo, hi);

            break;
        }
        case AddressingMode::INDIRECT_X: {
            auto& mem = System::get<Memory>();
            auto address = readFromPc();
            incPc(1);

            uint8_t lo = mem.readAddress(address + reg_.x, 0x00);
            uint8_t hi = mem.readAddress(address + reg_.x + 0x01, 0x00);

            retval = mem.readAddress(lo, hi);
            break;
        }
        case AddressingMode::NONE:
            // Ignore
            break;
        default:
            throw std::invalid_argument("READ: Unknown addressing mode. This should never happen");
            break;
    }

    return retval;
}

void Cpu::writeArgument(const InstructionData& instruction_data, unsigned int& /* cycles */, uint8_t value) {
    incPc(1); // Skip instruction byte
    auto& memory = System::get<Memory>();

    switch (instruction_data.addr_mode) {
        case AddressingMode::ABSOLUTE: {
            uint8_t lo = readFromPc();
            incPc(1);
            uint8_t hi = readFromPc();
            incPc(1);

            memory.writeAddress(lo, hi, value);
            break;
        }
        default:
            throw std::invalid_argument("WRITE: Unknown addressing mode. This should never happen");
            break;
    }
}

Cpu::Registers Cpu::getRegisters() {
    return reg_;
}
