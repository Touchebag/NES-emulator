#pragma once

#include <vector>
#include <optional>
#include <cstdint>

struct RomHeader {
    explicit RomHeader(std::vector<uint8_t> header);

    void printHeaderInfo();

    int mapper = -1;;

    uint8_t flags_6  = 0;
    uint8_t flags_7  = 0;
    uint8_t flags_8  = 0;
    uint8_t flags_9  = 0;
    uint8_t flags_10 = 0;

    uint8_t prg_rom_blocks = 0;
    uint8_t chr_rom_blocks = 0;

};
