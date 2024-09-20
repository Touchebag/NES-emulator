#pragma once

#include "rom.h"

namespace mappers {

class Mapper0 : public Rom {
  public:
    virtual uint8_t readAddress(uint16_t address) {
        address = address & 0x3FFF;
        return rom_.at(address);
    }

    virtual void writeAddress(uint16_t address, uint8_t value) {
        address = address & 0x3FFF;
        rom_.at(address) = value;
    }
};

} // namespace
