#include "memory.h"

#include <stdexcept>

#include "system.h"

#include "log.h"

constexpr uint16_t RAM = 0x2000; // RAM mirrors of 0x0000 - 0x07FF
constexpr uint16_t PPU = 0x4000; // PPU registers mirrors of 0x2008
constexpr uint16_t APU = 0x4018; // APU registers
constexpr uint16_t N_A = 0x401F; // Not used
constexpr uint16_t ROM = 0xFFFF; // Mapper space

uint8_t Memory::readAddress(uint8_t address_low, uint8_t address_high) {
    uint16_t address = (address_high << 8) | address_low;

    if (address < RAM) {
        // First 0x2000 bytes are mirrors of 0x0000 - 0x07FF
        return memory_[address & 0x7FF];
    } else if (address < PPU) {
        return System::get<Ppu>().readRegister(address & 0x07);
    } else if (address < APU) {
        // return &(apu_registers[address]);
    } else if (address < N_A) {
        // Disabled
        // return &(memory_[address]);
    } else if (address <= ROM) {
        // If rom is not loaded use direct memory (for testing)
        // TODO Change to fake mapper instead
        if (!rom_) {
            return memory_[address];
        }
        // Otherwise get mapped address
        if (address > 0x8000) {
            return *(rom_.value().getAddress(address & 0x3FFF));
        }
    } else {
        LOGE("Invalid memory address %x", address);
        throw std::runtime_error("");
    }

    return memory_[address];
};

void Memory::writeAddress(uint8_t address_low, uint8_t address_high, uint8_t value) {
    uint16_t address = (address_high << 8) | address_low;

    if (address < RAM) {
        // First 0x2000 bytes are mirrors of 0x0000 - 0x07FF
        memory_[address & 0x7FF] = value;
    } else if (address < PPU) {
        System::get<Ppu>().writeRegister(address & 0x07, value);
    } else if (address < APU) {
        // return &(apu_registers[address]);
    } else if (address < N_A) {
        // Disabled
        // return &(memory_[address]);
    } else if (address <= ROM) {
        // If rom is not loaded use direct memory (for testing)
        // TODO Change to fake mapper instead
        if (!rom_) {
            memory_[address] = value;
        }
        // Otherwise get mapped address
        if (address > 0x8000) {
            *(rom_.value().getAddress(address & 0x3FFF)) = value;
        }
    } else {
        LOGE("Invalid memory address %x", address);
        throw std::runtime_error("");
    }

    memory_[address] = value;

    return;
};

void Memory::loadRom(Rom rom) {
    rom_ = rom;
}
