#pragma once

#include <vector>
#include <cstdint>

#include "rom.h"

class Memory {
  public:
    uint8_t readAddress(uint8_t address_low, uint8_t address_high);
    void writeAddress(uint8_t address_low, uint8_t address_high, uint8_t value);

    void loadRom(Rom rom);

  private:
    uint8_t* getAddress(uint8_t lo, uint8_t hi);

    std::vector<uint8_t> memory_ = std::vector<uint8_t>(0x10000);

    Rom rom_;
};
