#pragma once

#include <cstdint>
#include <vector>

#include "palettes.h"

class Ppu {
  public:
    Ppu();

    void advance(int cycles);

    void writeRegister(uint16_t address, uint8_t value);
    uint8_t readRegister(uint16_t address);

    const std::vector<palettes::RGB>& getFramebuffer();

  private:
    void incrementCurrentAddress(uint8_t);

    uint8_t PPUCTRL   = 0;
    uint8_t PPUMASK   = 0;
    uint8_t PPUSTATUS = 0;
    uint8_t OAMADDR   = 0;
    uint8_t OAMDATA   = 0;
    uint8_t PPUSCROLL = 0;
    uint8_t PPUDATA   = 0;

    // Used to specify the next byte to write (upper/lower)
    // 0 = low
    // 1 = high
    int write_address_byte_ = 1;
    uint8_t current_address_[2] = {0x00, 0x00};


    unsigned int current_scanline_ = 0;
    unsigned int current_nametable_column_ = 0;

    unsigned int byte_cycle_count_ = 0;

    std::vector<uint8_t> vram_ = std::vector<uint8_t>(0x4000);

    std::vector<palettes::RGB> framebuffer_;
};
