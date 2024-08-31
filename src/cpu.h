#pragma once

#include "memory.h"

class Cpu {
  public:
    void executeInstruction(Memory& memory);

    void setPc(uint8_t pc_low, uint8_t pc_high);

    struct Registers {
      uint8_t pc[2];
      uint8_t sp;
      uint8_t p;
      uint8_t a;
      uint8_t x;
      uint8_t y;
    };
    Registers getRegisters();

  private:
    // CPU status flags
    enum class StatusFlag {
        CARRY = 1,
        ZERO = 2,
        INTERRUPT = 4,
        DECIMAL = 8,
        BREAK = 16,
        UNUSED = 32,
        OVERFLOW = 64,
        NEGATIVE = 128
    };

    void setStatusFlag(StatusFlag, bool);
    void setNegativeFlag(uint8_t);
    void setZeroFlag(uint8_t);

    uint8_t readFromPc(Memory&);

    void incPc(int);
    Registers reg_;
};
