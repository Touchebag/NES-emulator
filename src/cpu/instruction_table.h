#include <unordered_map>
#include <cstdint>

enum class AddressingMode {
    NONE,
    IMMEDIATE,
    RELATIVE,
    ABSOLUTE,
    ABSOLUTE_X,
    INDIRECT_X,
};

enum class InstructionType {
    NONE,
    BEQ,
    BNE,
    BRK,
    CLC,
    CMP,
    CPX,
    INX,
    LDA,
    LDX,
    JMP,
    RTI,
    SBC,
    STA,
};

struct InstructionData {
    unsigned int num_cycles = 0;
    AddressingMode addr_mode = AddressingMode::NONE;
    InstructionType type = InstructionType::NONE;
};

const std::unordered_map<uint8_t, InstructionData> instruction_table = {
    { 0xF0, { 2, AddressingMode::RELATIVE, InstructionType::BEQ }},

    { 0xD0, { 2, AddressingMode::RELATIVE, InstructionType::BNE }},

    { 0x00, { 2, AddressingMode::NONE, InstructionType::BRK }},

    { 0x18, { 2, AddressingMode::NONE, InstructionType::CLC }},

    { 0xC9, { 2, AddressingMode::IMMEDIATE, InstructionType::CMP }},

    { 0xE0, { 2, AddressingMode::IMMEDIATE, InstructionType::CPX }},

    { 0xE8, { 2, AddressingMode::NONE, InstructionType::INX }},

    { 0x4C, { 3, AddressingMode::NONE, InstructionType::JMP }},

    { 0xA9, { 2, AddressingMode::IMMEDIATE, InstructionType::LDA }},
    { 0xBD, { 4, AddressingMode::ABSOLUTE_X, InstructionType::LDA }},

    { 0xA2, { 2, AddressingMode::IMMEDIATE, InstructionType::LDX }},

    { 0x40, { 6, AddressingMode::NONE, InstructionType::RTI }},

    { 0xE1, { 6, AddressingMode::INDIRECT_X, InstructionType::SBC }},

    { 0x8D, { 4, AddressingMode::ABSOLUTE, InstructionType::STA }},
};
