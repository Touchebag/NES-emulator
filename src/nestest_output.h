#ifndef NESTEST_OUTPUT
#define NESTEST_OUTPUT

#include <cstdint>
#include <string>
#include <optional>

// This class is only intended for debugging pruposes during development
// It's intended function is to output text in a format usable for
// comparing logs when running nestest CPU tests

enum class AddressingMode;

struct NestestData {
    uint8_t pc_lo, pc_hi;
    uint8_t opcode;
    std::string name = "";
    std::optional<uint8_t> arg1 = std::nullopt;
    std::optional<uint8_t> arg2 = std::nullopt;
    AddressingMode mode;
    uint8_t reg_a, reg_x, reg_y, reg_p;
    uint8_t sp;
    int cycles;
    // Only used during special addressing modes
    uint8_t address_lo;
    uint8_t address_hi;
    uint8_t address_val;
};

void printNestestOutput(NestestData);

#endif
