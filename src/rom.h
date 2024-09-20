#pragma once

#include <string>
#include <vector>
#include <memory>

#include "rom_header.h"

class Rom {
  public:
    Rom();

    static std::shared_ptr<Rom> loadRomFromFile(const std::string& filepath);

    virtual uint8_t readAddress(uint16_t address);
    virtual void writeAddress(uint16_t address, uint8_t value);

  protected:
    RomHeader header_ = RomHeader{std::vector<uint8_t>{}};
    std::vector<uint8_t> rom_ = {};
};
