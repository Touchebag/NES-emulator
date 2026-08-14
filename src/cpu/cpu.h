#pragma once

#include <cstdint>
#include <optional>
#include <utility>

#include "instruction_table.h"
#include "log.h"

class InterpreterTestFixture;

class Cpu {
  public:
    Cpu();

    int executeInstruction();

    void setPc(uint8_t pc_low, uint8_t pc_high);

    // CPU status flags
    enum class StatusFlag : uint8_t {
        CARRY = 1,
        ZERO = 2,
        INTERRUPT = 4,
        DECIMAL = 8,
        BREAK = 16,
        UNUSED = 32,
        OVERFLOW = 64,
        NEGATIVE = 128
    };
    bool getStatusFlag(StatusFlag);

    struct Registers {
      uint8_t pc[2];
      uint8_t sp = 0;
      uint8_t p = 0;
      uint8_t a = 0;
      uint8_t x = 0;
      uint8_t y = 0;
    };
    Registers getRegisters();

#ifdef NESTEST_OUTPUT
    NestestData getPrevInstructionData();
#endif

  private:
    void setStatusFlag(StatusFlag, bool);
    void setNegativeFlag(uint8_t);
    void setZeroFlag(uint8_t);

    uint8_t readFromPc();

    void pushStack(uint8_t);
    uint8_t popStack();

    void incPc(int);
    Registers reg_;

    uint8_t readArgument(const InstructionData&, unsigned int& cycles);
    void writeArgument(const InstructionData&, unsigned int& cycles, uint8_t value);

#ifdef NESTEST_OUTPUT
    NestestData prev_instruction_data_;
#endif

    friend InterpreterTestFixture;
};
