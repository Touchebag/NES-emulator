#pragma once

#include <cstdint>

class InterpreterTestFixture;

class Cpu {
  public:
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
      uint8_t sp = 0xFF;
      uint8_t p;
      uint8_t a;
      uint8_t x;
      uint8_t y;
    };
    Registers getRegisters();
  private:

    void setStatusFlag(StatusFlag, bool);
    void setNegativeFlag(uint8_t);
    void setZeroFlag(uint8_t);

    uint8_t readFromPc();

    void pushStack(uint8_t);
    uint8_t popStack();

    void incPc(int);
    Registers reg_;

    friend InterpreterTestFixture;
};
