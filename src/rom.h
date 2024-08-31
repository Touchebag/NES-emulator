#pragma once

#include <string>
#include <vector>

class Rom {
  public:
    static Rom loadRomFromFile(const std::string& filepath);

    uint8_t* getAddress(uint16_t address);

  private:
    std::vector<uint8_t> rom_ = {};

    uint8_t flags_6_  = 0;
    uint8_t flags_7_  = 0;
    uint8_t flags_8_  = 0;
    uint8_t flags_9_  = 0;
    uint8_t flags_10_ = 0;

    uint8_t prg_rom_blocks_ = 0;
    uint8_t chr_rom_blocks_ = 0;

    void loadRom(const std::string& filepath);
    bool parseRomHeader();

    void printRomInfo();
};
