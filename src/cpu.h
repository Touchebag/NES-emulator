#pragma once

#include "memory.h"

class Cpu {
  public:
    void executeInstruction(Memory&);

    void setPc(uint8_t pc_low, uint8_t pc_high);

  private:
    uint8_t readFromPc(Memory&);

    struct Registers {
      uint8_t pc[2];
      uint8_t sp;
      uint8_t p;
      uint8_t a;
      uint8_t x;
      uint8_t y;
    };
    Registers reg_;
};
