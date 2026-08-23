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
    ABSOLUTE_Y,
    INDIRECT,
    INDIRECT_X,
    INDIRECT_Y,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    IMPLIED,
    ACCUMULATOR,
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
    { 0x69, { 2, AddressingMode::IMMEDIATE, InstructionType::ADC }},
    { 0x65, { 3, AddressingMode::ZERO_PAGE, InstructionType::ADC }},
    { 0x75, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::ADC }},
    { 0x6D, { 4, AddressingMode::ABSOLUTE, InstructionType::ADC }},
    { 0x7D, { 4, AddressingMode::ABSOLUTE_X, InstructionType::ADC }},
    { 0x79, { 4, AddressingMode::ABSOLUTE_Y, InstructionType::ADC }},
    { 0x61, { 6, AddressingMode::INDIRECT_X, InstructionType::ADC }},
    { 0x71, { 5, AddressingMode::INDIRECT_Y, InstructionType::ADC }},

    { 0x29, { 2, AddressingMode::IMMEDIATE, InstructionType::AND }},
    { 0x25, { 3, AddressingMode::ZERO_PAGE, InstructionType::AND }},
    { 0x35, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::AND }},
    { 0x2D, { 4, AddressingMode::ABSOLUTE, InstructionType::AND }},
    { 0x3D, { 4, AddressingMode::ABSOLUTE_X, InstructionType::AND }},
    { 0x39, { 4, AddressingMode::ABSOLUTE_Y, InstructionType::AND }},
    { 0x21, { 6, AddressingMode::INDIRECT_X, InstructionType::AND }},
    { 0x31, { 5, AddressingMode::INDIRECT_Y, InstructionType::AND }},

    { 0x0A, { 2, AddressingMode::ACCUMULATOR, InstructionType::ASL }},
    { 0x06, { 5, AddressingMode::ZERO_PAGE, InstructionType::ASL }},
    { 0x16, { 6, AddressingMode::ZERO_PAGE_X, InstructionType::ASL }},
    { 0x0E, { 6, AddressingMode::ABSOLUTE, InstructionType::ASL }},
    { 0x1E, { 7, AddressingMode::ABSOLUTE_X, InstructionType::ASL }},

    { 0x90, { 2, AddressingMode::RELATIVE, InstructionType::BCC }},

    { 0xB0, { 2, AddressingMode::RELATIVE, InstructionType::BCS }},

    { 0xF0, { 2, AddressingMode::RELATIVE, InstructionType::BEQ }},

    { 0x24, { 3, AddressingMode::ZERO_PAGE, InstructionType::BIT }},
    { 0x2C, { 4, AddressingMode::ABSOLUTE, InstructionType::BIT }},

    { 0x30, { 2, AddressingMode::RELATIVE, InstructionType::BMI }},

    { 0xD0, { 2, AddressingMode::RELATIVE, InstructionType::BNE }},

    { 0x10, { 2, AddressingMode::RELATIVE, InstructionType::BPL }},

    { 0x00, { 2, AddressingMode::IMPLIED, InstructionType::BRK }},

    { 0x50, { 2, AddressingMode::RELATIVE, InstructionType::BVC }},

    { 0x70, { 2, AddressingMode::RELATIVE, InstructionType::BVS }},

    { 0x18, { 2, AddressingMode::IMPLIED, InstructionType::CLC }},

    { 0xD8, { 2, AddressingMode::IMPLIED, InstructionType::CLD }},

    { 0x58, { 2, AddressingMode::IMPLIED, InstructionType::CLI }},

    { 0xB8, { 2, AddressingMode::IMPLIED, InstructionType::CLV }},

    { 0xC9, { 2, AddressingMode::IMMEDIATE, InstructionType::CMP }},
    { 0xC5, { 3, AddressingMode::ZERO_PAGE, InstructionType::CMP }},
    { 0xD5, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::CMP }},
    { 0xCD, { 4, AddressingMode::ABSOLUTE, InstructionType::CMP }},
    { 0xDD, { 4, AddressingMode::ABSOLUTE_X, InstructionType::CMP }},
    { 0xD9, { 4, AddressingMode::ABSOLUTE_Y, InstructionType::CMP }},
    { 0xC1, { 6, AddressingMode::INDIRECT_X, InstructionType::CMP }},
    { 0xD1, { 5, AddressingMode::INDIRECT_Y, InstructionType::CMP }},

    { 0xE0, { 2, AddressingMode::IMMEDIATE, InstructionType::CPX }},
    { 0xE4, { 3, AddressingMode::ZERO_PAGE, InstructionType::CPX }},
    { 0xEC, { 4, AddressingMode::ABSOLUTE, InstructionType::CPX }},

    { 0xC0, { 2, AddressingMode::IMMEDIATE, InstructionType::CPY }},
    { 0xC4, { 3, AddressingMode::ZERO_PAGE, InstructionType::CPY }},
    { 0xCC, { 4, AddressingMode::ABSOLUTE, InstructionType::CPY }},

    { 0xC6, { 5, AddressingMode::ZERO_PAGE, InstructionType::DEC }},
    { 0xD6, { 6, AddressingMode::ZERO_PAGE_X, InstructionType::DEC }},
    { 0xCE, { 6, AddressingMode::ABSOLUTE, InstructionType::DEC }},
    { 0xDE, { 7, AddressingMode::ABSOLUTE_X, InstructionType::DEC }},

    { 0xCA, { 2, AddressingMode::IMPLIED, InstructionType::DEX }},

    { 0x88, { 2, AddressingMode::IMPLIED, InstructionType::DEY }},

    { 0x49, { 2, AddressingMode::IMMEDIATE, InstructionType::EOR }},
    { 0x45, { 3, AddressingMode::ZERO_PAGE, InstructionType::EOR }},
    { 0x55, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::EOR }},
    { 0x4D, { 4, AddressingMode::ABSOLUTE, InstructionType::EOR }},
    { 0x5D, { 4, AddressingMode::ABSOLUTE_X, InstructionType::EOR }},
    { 0x59, { 4, AddressingMode::ABSOLUTE_Y, InstructionType::EOR }},
    { 0x41, { 6, AddressingMode::INDIRECT_X, InstructionType::EOR }},
    { 0x51, { 5, AddressingMode::INDIRECT_Y, InstructionType::EOR }},

    { 0xE6, { 5, AddressingMode::ZERO_PAGE, InstructionType::INC }},
    { 0xF6, { 6, AddressingMode::ZERO_PAGE_X, InstructionType::INC }},
    { 0xEE, { 6, AddressingMode::ABSOLUTE, InstructionType::INC }},
    { 0xFE, { 7, AddressingMode::ABSOLUTE_X, InstructionType::INC }},

    { 0xE8, { 2, AddressingMode::IMPLIED, InstructionType::INX }},

    { 0xC8, { 2, AddressingMode::IMPLIED, InstructionType::INY }},

    { 0x4C, { 3, AddressingMode::ABSOLUTE, InstructionType::JMP }},
    { 0x6C, { 3, AddressingMode::INDIRECT, InstructionType::JMP }},

    { 0x20, { 6, AddressingMode::ABSOLUTE, InstructionType::JSR }},

    { 0xA9, { 2, AddressingMode::IMMEDIATE, InstructionType::LDA }},
    { 0xA5, { 3, AddressingMode::ZERO_PAGE, InstructionType::LDA }},
    { 0xB5, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::LDA }},
    { 0xAD, { 4, AddressingMode::ABSOLUTE, InstructionType::LDA }},
    { 0xBD, { 4, AddressingMode::ABSOLUTE_X, InstructionType::LDA }},
    { 0xB9, { 4, AddressingMode::ABSOLUTE_Y, InstructionType::LDA }},
    { 0xA1, { 6, AddressingMode::INDIRECT_X, InstructionType::LDA }},
    { 0xB1, { 5, AddressingMode::INDIRECT_Y, InstructionType::LDA }},

    { 0xA2, { 2, AddressingMode::IMMEDIATE, InstructionType::LDX }},
    { 0xA6, { 3, AddressingMode::ZERO_PAGE, InstructionType::LDX }},
    { 0xB6, { 4, AddressingMode::ZERO_PAGE_Y, InstructionType::LDX }},
    { 0xAE, { 4, AddressingMode::ABSOLUTE, InstructionType::LDX }},
    { 0xBE, { 4, AddressingMode::ABSOLUTE_Y, InstructionType::LDX }},

    { 0xA0, { 2, AddressingMode::IMMEDIATE, InstructionType::LDY }},
    { 0xA4, { 3, AddressingMode::ZERO_PAGE, InstructionType::LDY }},
    { 0xB4, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::LDY }},
    { 0xAC, { 4, AddressingMode::ABSOLUTE, InstructionType::LDY }},
    { 0xBC, { 4, AddressingMode::ABSOLUTE_X, InstructionType::LDY }},

    { 0x4A, { 2, AddressingMode::ACCUMULATOR, InstructionType::LSR }},
    { 0x46, { 5, AddressingMode::ZERO_PAGE, InstructionType::LSR }},
    { 0x56, { 6, AddressingMode::ZERO_PAGE_X, InstructionType::LSR }},
    { 0x4E, { 6, AddressingMode::ABSOLUTE, InstructionType::LSR }},
    { 0x5E, { 7, AddressingMode::ABSOLUTE_X, InstructionType::LSR }},

    { 0xEA, { 2, AddressingMode::IMPLIED, InstructionType::NOP }},

    { 0x09, { 2, AddressingMode::IMMEDIATE, InstructionType::ORA }},
    { 0x05, { 3, AddressingMode::ZERO_PAGE, InstructionType::ORA }},
    { 0x15, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::ORA }},
    { 0x0D, { 4, AddressingMode::ABSOLUTE, InstructionType::ORA }},
    { 0x1D, { 4, AddressingMode::ABSOLUTE_X, InstructionType::ORA }},
    { 0x19, { 4, AddressingMode::ABSOLUTE_Y, InstructionType::ORA }},
    { 0x01, { 6, AddressingMode::INDIRECT_X, InstructionType::ORA }},
    { 0x11, { 5, AddressingMode::INDIRECT_Y, InstructionType::ORA }},

    { 0x48, { 3, AddressingMode::IMPLIED, InstructionType::PHA }},

    { 0x08, { 3, AddressingMode::IMPLIED, InstructionType::PHP }},

    { 0x68, { 4, AddressingMode::IMPLIED, InstructionType::PLA }},

    { 0x28, { 4, AddressingMode::IMPLIED, InstructionType::PLP }},

    { 0x2A, { 2, AddressingMode::ACCUMULATOR, InstructionType::ROL }},
    { 0x26, { 5, AddressingMode::ZERO_PAGE, InstructionType::ROL }},
    { 0x36, { 6, AddressingMode::ZERO_PAGE_X, InstructionType::ROL }},
    { 0x2E, { 6, AddressingMode::ABSOLUTE, InstructionType::ROL }},
    { 0x3E, { 7, AddressingMode::ABSOLUTE_X, InstructionType::ROL }},

    { 0x6A, { 2, AddressingMode::ACCUMULATOR, InstructionType::ROR }},
    { 0x66, { 5, AddressingMode::ZERO_PAGE, InstructionType::ROR }},
    { 0x76, { 6, AddressingMode::ZERO_PAGE_X, InstructionType::ROR }},
    { 0x6E, { 6, AddressingMode::ABSOLUTE, InstructionType::ROR }},
    { 0x7E, { 7, AddressingMode::ABSOLUTE_X, InstructionType::ROR }},

    { 0x40, { 6, AddressingMode::IMPLIED, InstructionType::RTI }},

    { 0x60, { 6, AddressingMode::IMPLIED, InstructionType::RTS }},

    { 0xE9, { 2, AddressingMode::IMMEDIATE, InstructionType::SBC }},
    { 0xE5, { 3, AddressingMode::ZERO_PAGE, InstructionType::SBC }},
    { 0xF5, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::SBC }},
    { 0xED, { 4, AddressingMode::ABSOLUTE, InstructionType::SBC }},
    { 0xFD, { 4, AddressingMode::ABSOLUTE_X, InstructionType::SBC }},
    { 0xF9, { 4, AddressingMode::ABSOLUTE_Y, InstructionType::SBC }},
    { 0xE1, { 6, AddressingMode::INDIRECT_X, InstructionType::SBC }},
    { 0xF1, { 5, AddressingMode::INDIRECT_Y, InstructionType::SBC }},

    { 0x38, { 2, AddressingMode::IMPLIED, InstructionType::SEC }},

    { 0xF8, { 2, AddressingMode::IMPLIED, InstructionType::SED }},

    { 0x78, { 2, AddressingMode::IMPLIED, InstructionType::SEI }},

    { 0x8D, { 4, AddressingMode::ABSOLUTE, InstructionType::STA }},
    { 0x85, { 3, AddressingMode::ZERO_PAGE, InstructionType::STA }},

    { 0x86, { 3, AddressingMode::ZERO_PAGE, InstructionType::STX }},
    { 0x96, { 4, AddressingMode::ZERO_PAGE_Y, InstructionType::STX }},
    { 0x8E, { 4, AddressingMode::ABSOLUTE, InstructionType::STX }},

    { 0x94, { 4, AddressingMode::ZERO_PAGE_X, InstructionType::STY }},

    { 0xAA, { 2, AddressingMode::IMPLIED, InstructionType::TAX }},

    { 0xA8, { 2, AddressingMode::IMPLIED, InstructionType::TAY }},

    { 0xBA, { 2, AddressingMode::IMPLIED, InstructionType::TSX }},

    { 0x8A, { 2, AddressingMode::IMPLIED, InstructionType::TXA }},

    { 0x9A, { 2, AddressingMode::IMPLIED, InstructionType::TXS }},

    { 0x98, { 2, AddressingMode::IMPLIED, InstructionType::TYA }},
};

#endif
