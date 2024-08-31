#include "memory.h"

#include <stdexcept>

#include "log.h"

uint8_t Memory::readAddress(uint8_t address_low, uint8_t address_high) {
    auto location = getAddress(address_low, address_high);

    return *location;
};

void Memory::writeAddress(uint8_t address_low, uint8_t address_high, uint8_t value) {
    auto location = getAddress(address_low, address_high);
    *location = value;

    return;
};

void Memory::loadRom(Rom rom) {
    rom_ = rom;
}

uint8_t* Memory::getAddress(uint8_t address_low, uint8_t address_high) {
    uint16_t address = (address_high << 8) | address_low;

    if (address < 0x2000) {
        // First 0x2000 bytes are mirrors of 0x0000 - 0x07FF
        return &(memory_[address & 0x7FF]);
    } else if (address < 0x4000) {
        // return &(ppu_registers[address & 0x7]);
    } else if (address < 0x4018) {
        // return &(apu_registers[address]);
    } else if (address < 0x401F) {
        // Disabled
        // return &(memory_[address]);
    } else if (address < 0x10000) {
        // Get mapped address
        if (address > 0x8000) {
            return rom_.getAddress(address & 0x3FFF);
        }
    } else {
        LOGE("Invalid memory address %x", address);
        throw std::runtime_error("");
    }

    return &(memory_[address]);
}
