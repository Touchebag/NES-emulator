#ifndef INSTRUCTION_TABLE
#define INSTRUCTION_TABLE

#include <unordered_map>
#include <cstdint>
#include <string>

enum class AddressingMode {
    NONE,
    IMMEDIATE,
    RELATIVE,
    ABSOLUTE,
    ABSOLUTE_X,
    INDIRECT_X,
    ZERO_PAGE,
    IMPLIED,
};

#define INSTRUCTION_SET \
    GENERATE_ENUM(NONE),\
    GENERATE_ENUM(ADC),\
    GENERATE_ENUM(AND),\
    GENERATE_ENUM(ASL),\
    GENERATE_ENUM(BCC),\
    GENERATE_ENUM(BCS),\
    GENERATE_ENUM(BEQ),\
    GENERATE_ENUM(BIT),\
    GENERATE_ENUM(BMI),\
    GENERATE_ENUM(BNE),\
    GENERATE_ENUM(BPL),\
    GENERATE_ENUM(BRK),\
    GENERATE_ENUM(BVC),\
    GENERATE_ENUM(BVS),\
    GENERATE_ENUM(CLC),\
    GENERATE_ENUM(CLD),\
    GENERATE_ENUM(CLI),\
    GENERATE_ENUM(CLV),\
    GENERATE_ENUM(CMP),\
    GENERATE_ENUM(CPX),\
    GENERATE_ENUM(CPY),\
    GENERATE_ENUM(DEC),\
    GENERATE_ENUM(DEX),\
    GENERATE_ENUM(DEY),\
    GENERATE_ENUM(EOR),\
    GENERATE_ENUM(INC),\
    GENERATE_ENUM(INX),\
    GENERATE_ENUM(INY),\
    GENERATE_ENUM(JMP),\
    GENERATE_ENUM(JSR),\
    GENERATE_ENUM(LDA),\
    GENERATE_ENUM(LDX),\
    GENERATE_ENUM(LDY),\
    GENERATE_ENUM(LSR),\
    GENERATE_ENUM(NOP),\
    GENERATE_ENUM(ORA),\
    GENERATE_ENUM(PHA),\
    GENERATE_ENUM(PHP),\
    GENERATE_ENUM(PLA),\
    GENERATE_ENUM(PLP),\
    GENERATE_ENUM(ROL),\
    GENERATE_ENUM(ROR),\
    GENERATE_ENUM(RTI),\
    GENERATE_ENUM(RTS),\
    GENERATE_ENUM(SBC),\
    GENERATE_ENUM(SEC),\
    GENERATE_ENUM(SED),\
    GENERATE_ENUM(SEI),\
    GENERATE_ENUM(STA),\
    GENERATE_ENUM(STX),\
    GENERATE_ENUM(STY),\
    GENERATE_ENUM(TAX),\
    GENERATE_ENUM(TAY),\
    GENERATE_ENUM(TSX),\
    GENERATE_ENUM(TXA),\
    GENERATE_ENUM(TXS),\
    GENERATE_ENUM(TYA)

enum class InstructionType {
#define GENERATE_ENUM(x) x
INSTRUCTION_SET
#undef GENERATE_ENUM
};

inline std::unordered_map<InstructionType, std::string> InstructionStringMap {
#define GENERATE_ENUM(x) {InstructionType::x, #x}
INSTRUCTION_SET
#undef GENERATE_ENUM
};

struct InstructionData {
    unsigned int num_cycles = 0;
    AddressingMode addr_mode = AddressingMode::NONE;
    InstructionType type = InstructionType::NONE;
};

const std::unordered_map<uint8_t, InstructionData> instruction_table = {
    { 0x29, { 2, AddressingMode::IMMEDIATE, InstructionType::AND }},

    { 0x90, { 2, AddressingMode::RELATIVE, InstructionType::BCC }},

    { 0xB0, { 2, AddressingMode::RELATIVE, InstructionType::BCS }},

    { 0xF0, { 2, AddressingMode::RELATIVE, InstructionType::BEQ }},

    { 0x24, { 3, AddressingMode::ZERO_PAGE, InstructionType::BIT }},

    { 0xD0, { 2, AddressingMode::RELATIVE, InstructionType::BNE }},

    { 0x10, { 2, AddressingMode::RELATIVE, InstructionType::BPL }},

    { 0x00, { 2, AddressingMode::IMPLIED, InstructionType::BRK }},

    { 0x50, { 2, AddressingMode::RELATIVE, InstructionType::BVC }},

    { 0x70, { 2, AddressingMode::RELATIVE, InstructionType::BVS }},

    { 0x18, { 2, AddressingMode::IMPLIED, InstructionType::CLC }},

    { 0xC9, { 2, AddressingMode::IMMEDIATE, InstructionType::CMP }},

    { 0xE0, { 2, AddressingMode::IMMEDIATE, InstructionType::CPX }},

    { 0xE8, { 2, AddressingMode::IMPLIED, InstructionType::INX }},

    { 0x4C, { 3, AddressingMode::ABSOLUTE, InstructionType::JMP }},

    { 0x20, { 6, AddressingMode::ABSOLUTE, InstructionType::JSR }},

    { 0xA9, { 2, AddressingMode::IMMEDIATE, InstructionType::LDA }},
    { 0xBD, { 4, AddressingMode::ABSOLUTE_X, InstructionType::LDA }},

    { 0xA2, { 2, AddressingMode::IMMEDIATE, InstructionType::LDX }},

    { 0xEA, { 2, AddressingMode::IMPLIED, InstructionType::NOP }},

    { 0x08, { 3, AddressingMode::IMPLIED, InstructionType::PHP }},

    { 0x68, { 4, AddressingMode::IMPLIED, InstructionType::PLA }},

    { 0x40, { 6, AddressingMode::IMPLIED, InstructionType::RTI }},

    { 0x60, { 6, AddressingMode::IMPLIED, InstructionType::RTS }},

    { 0xE1, { 6, AddressingMode::INDIRECT_X, InstructionType::SBC }},

    { 0x38, { 2, AddressingMode::IMPLIED, InstructionType::SEC }},

    { 0xF8, { 2, AddressingMode::IMPLIED, InstructionType::SED }},

    { 0x78, { 2, AddressingMode::IMPLIED, InstructionType::SEI }},

    { 0x8D, { 4, AddressingMode::ABSOLUTE, InstructionType::STA }},
    { 0x85, { 3, AddressingMode::ZERO_PAGE, InstructionType::STA }},

    { 0x86, { 3, AddressingMode::ZERO_PAGE, InstructionType::STX }},
};

#endif
