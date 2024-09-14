#pragma once

#include <vector>
#include <cstdint>
#include <optional>

#include "rom.h"

class InterpreterTestFixture;

class Memory {
  public:
    uint8_t readAddress(uint8_t address_low, uint8_t address_high);
    void writeAddress(uint8_t address_low, uint8_t address_high, uint8_t value);

    void loadRom(Rom rom);

  private:
    std::vector<uint8_t> memory_ = std::vector<uint8_t>(0x10000);

    std::optional<Rom> rom_ = std::nullopt;

    friend InterpreterTestFixture;
};
