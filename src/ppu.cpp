#include "ppu.h"

#include <stdexcept>
#include <iostream>
#include <random>

#include "system.h"

#include "log.h"

namespace {

palettes::RGB lookupRgbValue(uint8_t index) {
    if (index >= palettes::def.size()) {
        LOGE("Palette index %x out of range", index);
        throw std::runtime_error("");
    }

    return (palettes::def[index]);
}

} // namespace

Ppu::Ppu() {
    std::random_device rd;
    std::mt19937 rnd_gen(rd());
    std::uniform_int_distribution<> distr(0, 63);

    for (auto x = 0; x < 244; x++) {
        for (auto y = 0; y < 256; y++) {
            framebuffer_.push_back(lookupRgbValue(1));
        }
    }
}

void Ppu::advance(int cycles) {
    // Add leftover cycles from previous call
    cycles += byte_cycle_count_;

    while (cycles > 7) {
        cycles -= 8;

        for (auto i = 0; i < 7; i++) {
            auto current_nametable_index = (current_scanline_ / 8) * 32 + current_nametable_column_;
            framebuffer_.at(current_nametable_index) = lookupRgbValue(vram_.at(0x2000 + current_nametable_index));
            // printf("%x ", vram_.at(0x2000 + current_nametable_index_));
        }

        if (++current_nametable_column_ >= 32) {
            current_nametable_column_ = 0;

            if (++current_scanline_ >= 240) {
                System::getInstance().onVsyncTriggered();
                current_scanline_ = 0;
            }
        }
    }

    // Store leftovers
    byte_cycle_count_ = cycles;
}

void Ppu::incrementCurrentAddress(uint8_t bytes) {
    uint8_t old_lo = current_address_[0];

    current_address_[0] += bytes;

    if (current_address_[0] < old_lo) {
        // Wraparound, increment hi
        current_address_[1]++;
    }
}

void Ppu::writeRegister(uint16_t address, uint8_t value) {
    switch (address) {
        case 0x0:
            LOGD("W PPUCTRL %x", value);
            PPUCTRL = value;
            break;
        case 0x1:
            LOGD("W PPUMASK %x", value);
            PPUMASK = value;
            break;
        case 0x2:
            LOGD("W PPUSTATUS %x", value);
            PPUSTATUS = value;
            break;
        case 0x3:
            LOGD("W OAMADDR %x", value);
            OAMADDR = value;
            break;
        case 0x4:
            LOGD("W OAMDATA %x", value);
            OAMDATA = value;
            break;
        case 0x5:
            LOGD("W PPUSCROLL %x", value);
            PPUSCROLL = value;
            break;
        case 0x6:
            current_address_[write_address_byte_] = value;
            write_address_byte_ = 1 - write_address_byte_;
            break;
        case 0x7:
            PPUDATA = value;
            if (vram_[0x2000] == 0) {
                incrementCurrentAddress(1);
            } else {
                incrementCurrentAddress(32);
            }
            break;
        default:
            LOGE("Invalid PPU register address %x", address);
            throw std::runtime_error("");
    }
}

uint8_t Ppu::readRegister(uint16_t address) {
    switch (address) {
        case 0x0:
            LOGD("R PPUCTRL");
            return PPUCTRL;
        case 0x1:
            LOGD("R PPUMASK");
            return PPUMASK;
        case 0x2:
            LOGD("R PPUSTATUS");
            return PPUSTATUS;
        case 0x3:
            LOGD("R OAMADDR");
            return OAMADDR;
        case 0x4:
            LOGD("R OAMDATA");
            return OAMDATA;
        case 0x5:
            LOGD("R PPUSCROLL");
            return PPUSCROLL;
        case 0x6:
            LOGE("PPUADDR not readable");
            throw std::runtime_error("");
        case 0x7:
            LOGD("R PPUDATA");
            return PPUDATA;
        default:
            LOGE("Invalid PPU register address %x", address);
            throw std::runtime_error("");
    }
}

const std::vector<palettes::RGB>& Ppu::getFramebuffer() {
    return framebuffer_;
}
