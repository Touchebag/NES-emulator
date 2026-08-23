#include "parsed_instruction.h"

#include "system.h"

#include <stdexcept>
#include <format>

namespace {

uint8_t read_mem_at_pc(uint8_t lo, uint8_t hi, uint8_t offset = 0) {
    auto tmp_lo = lo;

    tmp_lo += offset;
    if (tmp_lo < lo) {
        // Overflow happened
        hi++;
    }

    return System::getInstance().get<Memory>().readAddress(tmp_lo, hi);
}

std::unordered_map<AddressingMode, int> addressing_mode_args_map {
    {AddressingMode::IMMEDIATE, 2},
    {AddressingMode::RELATIVE, 2},
    {AddressingMode::ABSOLUTE, 3},
    {AddressingMode::ABSOLUTE_X, 3},
    {AddressingMode::ABSOLUTE_Y, 3},
    {AddressingMode::INDIRECT_X, 2},
    {AddressingMode::ZERO_PAGE, 2},
    {AddressingMode::ZERO_PAGE_X, 2},
    {AddressingMode::ZERO_PAGE_Y, 2},
    {AddressingMode::IMPLIED, 1},
};

} // namespace

ParsedInstruction::ParsedInstruction(uint8_t lo, uint8_t hi) {
    pc_lo_ = lo;
    pc_hi_ = hi;

    auto& cpu = System::getInstance().get<Cpu>();

    opcode_ = read_mem_at_pc(pc_lo_, pc_hi_);

    try {
        auto instruction_data = instruction_table.at(opcode_);

        type_ = instruction_data.type;
        cycles_ = instruction_data.num_cycles;
        addressing_mode_ = instruction_data.addr_mode;
    } catch(std::out_of_range& e) {
        LOGE("Uknown opcode 0x%02X", opcode_);
        throw e;
    }

    auto regs = cpu.getRegisters();
    reg_a_ = regs.a;
    reg_x_ = regs.x;
    reg_y_ = regs.y;
    reg_p_ = regs.p;
    sp_ = regs.sp;

    name_ = InstructionStringMap.at(type_);
    num_args_ = addressing_mode_args_map.at(addressing_mode_);

    if (num_args_ > 1) {
        arg1_ = {read_mem_at_pc(pc_lo_, pc_hi_, 1)};
    }
    if (num_args_ > 2) {
        arg2_ = {read_mem_at_pc(pc_lo_, pc_hi_, 2)};
    }

    calcualateAdjustedMemoryAddresses();
}

void ParsedInstruction::calcualateAdjustedMemoryAddresses() {
    adjusted_lo_ = 0x00;
    adjusted_hi_ = 0x00;

    switch (addressing_mode_) {
        case AddressingMode::RELATIVE:
            adjusted_hi_ = pc_hi_;

            adjusted_lo_ = pc_lo_ + 2 + arg1_.value();
            // If relative is negative
            if (arg1_.value() & 128) {
                if (adjusted_lo_ > pc_lo_) {
                    // Underflow, carry to hi;
                    adjusted_hi_--;
                    cycles_++;
                }
            } else if (adjusted_lo_ < pc_lo_) {
                // Overflow, carry to hi;
                adjusted_hi_++;
                cycles_++;
            }

            val1_ = arg1_;

            break;
        case AddressingMode::IMMEDIATE:
            val1_ = arg1_;

            break;
        case AddressingMode::ABSOLUTE:
            adjusted_lo_ = arg1_.value();
            adjusted_hi_ = arg2_.value();
            break;
        case AddressingMode::ABSOLUTE_X: {
            adjusted_lo_ = arg1_.value() + reg_x_;
            adjusted_hi_ = arg2_.value();

            if (adjusted_lo_ < arg1_.value()) {
                // Overflow, carry to hi
                adjusted_hi_++;
                // Page cross, extra cycle
                cycles_++;
            }

            val1_ = System::get<Memory>().readAddress(adjusted_lo_, adjusted_hi_);

            break;
        }
        case AddressingMode::ABSOLUTE_Y: {
            adjusted_lo_ = arg1_.value() + reg_y_;
            adjusted_hi_ = arg2_.value();

            if (adjusted_lo_ < arg1_.value()) {
                // Overflow, carry to hi
                adjusted_hi_++;
                // Page cross, extra cycle
                cycles_++;
            }

            val1_ = System::get<Memory>().readAddress(adjusted_lo_, adjusted_hi_);

            break;
        }
        case AddressingMode::INDIRECT_X: {
            auto& mem = System::get<Memory>();

            adjusted_lo_ = mem.readAddress(arg1_.value() + reg_x_, 0x00);
            adjusted_hi_ = mem.readAddress(arg1_.value() + reg_x_ + 0x01, 0x00);

            val1_ = mem.readAddress(adjusted_lo_, adjusted_hi_);
            break;
        }
        case AddressingMode::ZERO_PAGE: {
            auto& mem = System::get<Memory>();

            adjusted_lo_ = arg1_.value();
            adjusted_hi_ = 0x00;

            val1_ = mem.readAddress(adjusted_lo_, adjusted_hi_);
            break;
        }
        case AddressingMode::ZERO_PAGE_X: {
            auto& mem = System::get<Memory>();

            // Uint8 type handles wraparound
            adjusted_lo_ = arg1_.value() + reg_x_;
            // Zero page, no carry needed
            adjusted_hi_ = 0x00;

            val1_ = mem.readAddress(adjusted_lo_, adjusted_hi_);
            break;
        }
        case AddressingMode::ZERO_PAGE_Y: {
            auto& mem = System::get<Memory>();

            // Uint8 type handles wraparound
            adjusted_lo_ = arg1_.value() + reg_y_;
            // Zero page, no carry needed
            adjusted_hi_ = 0x00;

            val1_ = mem.readAddress(adjusted_lo_, adjusted_hi_);
            break;
        }
        case AddressingMode::IMPLIED:
            // Do nothing
            break;
        default:
            LOGE("Adressing mode %i", static_cast<int>(addressing_mode_));
            throw std::invalid_argument("PARSED_INSTRUCTION_READ: Unknown addressing mode. This should never happen");
            break;
    }
}

std::string ParsedInstruction::toString(int extra_cycles) const {
    using std::format;

    std::string output;

    output += format("{:02X}{:02X}  ", pc_hi_, pc_lo_);
    output += format("{:02X} ", opcode_);

    // Optional arguments
    if (arg1_) {
        output += format("{:02X} ", arg1_.value());

        if (arg2_) {
            output += format("{:02X} ", arg2_.value());
        } else {
            output += format("   ");
        }
    } else {
        output += format("      ");
    }
    output += format(" {} ", name_);

    switch (addressing_mode_) {
        case AddressingMode::RELATIVE:
            output += format("${:02X}{:02X}                       ", adjusted_hi_, adjusted_lo_);
            break;
        case AddressingMode::IMMEDIATE:
            output += format("#${:02X}                        ", arg1_.value());
            break;
        case AddressingMode::ABSOLUTE:
            output += format("${:02X}{:02X}                       ", arg2_.value(), arg1_.value());
            break;
        case AddressingMode::ABSOLUTE_X:
            output += format("${:02X}{:02X},X @ {:02X}{:02X} = {:02X}         ",
                    arg2_.value(),
                    arg1_.value(),
                    adjusted_hi_,
                    adjusted_lo_,
                    val1_.value());
            break;
        case AddressingMode::ABSOLUTE_Y:
            output += format("${:02X}{:02X},X @ {:02X}{:02X} = {:02X}         ",
                    arg2_.value(),
                    arg1_.value(),
                    adjusted_hi_,
                    adjusted_lo_,
                    val1_.value());
            break;
        case AddressingMode::INDIRECT_X:
            output += format("INDIRECT_X");
            break;
        case AddressingMode::ZERO_PAGE:
            output += format("${:02X} = {:02X}                    ", adjusted_lo_, val1_.value());
            break;
        case AddressingMode::ZERO_PAGE_X:
            output += format("${:02X},Y @ {:02X} = {:02X}             ", arg1_.value(), adjusted_lo_, val1_.value());
            break;
        case AddressingMode::ZERO_PAGE_Y:
            output += format("${:02X},Y @ {:02X} = {:02X}             ", arg1_.value(), adjusted_lo_, val1_.value());
            break;
        case AddressingMode::IMPLIED:
            output += format("                            ");
            break;
        default:
            throw std::invalid_argument("PARSED_INSTRUCTION_PRINT: Unknown addressing mode. This should never happen");
            break;
    }

    output += format("A:{:02X} X:{:02X} Y:{:02X} P:{:02X} SP:{:02X} ", reg_a_, reg_x_, reg_y_, reg_p_, sp_);

    auto ppu_position = System::get<Ppu>().getCurrentPosition();
    output += format("PPU:{:3},{:3} ", ppu_position.first, ppu_position.second);
    output += format("CYC:{}", extra_cycles);

    return output;
}
