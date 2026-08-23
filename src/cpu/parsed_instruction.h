#ifndef PARSED_INSTRUCTION
#define PARSED_INSTRUCTION

#include <cstdint>
#include <string>
#include <optional>

#include "instruction_table.h"

class ParsedInstruction {
  public:
    ParsedInstruction(uint8_t lo, uint8_t hi);

    std::string toString(int current_total_cycles = 0) const;

    void handlePageCross();

    uint8_t pc_lo_;
    uint8_t pc_hi_;

    uint8_t opcode_;
    InstructionType type_;
    std::string name_ = "";

    AddressingMode addressing_mode_;
    std::optional<uint8_t> arg1_ = std::nullopt;
    std::optional<uint8_t> arg2_ = std::nullopt;
    uint8_t num_args_;

    uint8_t reg_a_;
    uint8_t reg_x_;
    uint8_t reg_y_;
    uint8_t reg_p_;
    uint8_t sp_;

    int cycles_;

    // Output values
    std::optional<uint8_t> val1_ = std::nullopt;
    std::optional<uint8_t> val2_ = std::nullopt;

    // Adjusted memory values after accounting for addressing modes
    uint8_t adjusted_lo_;
    uint8_t adjusted_hi_;

  private:
    void calcualateAdjustedMemoryAddresses();

    bool page_cross_ = false;
};

void printNestestOutput(ParsedInstruction, int total_cycles);

#endif
