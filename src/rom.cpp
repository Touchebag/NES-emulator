#include "rom.h"

#include <fstream>

#include "log.h"

Rom Rom::loadRomFromFile(const std::string& filepath) {
    Rom rom;
    rom.loadRom(filepath);
    return rom;
}

void Rom::loadRom(const std::string& filepath) {
    std::ifstream infile(filepath, std::ios_base::binary);
    if (infile.fail()) {
        throw std::invalid_argument(std::string("Could not open ROM file ") + filepath);
    }
    rom_ = std::vector<uint8_t>(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
    infile.close();

    if (!parseRomHeader()) {
        throw std::invalid_argument(std::string("Failed to parse ROM header"));
    }

    printf("ROM file %s loaded successfully\n", filepath.c_str());
    printRomInfo();
}

uint8_t* Rom::getAddress(uint16_t address) {
    if (address < rom_.size()) {
        return &(rom_[address + 0x10]); // Offset for header
    } else {
        LOGE("ROM address %x out of range", address);
        throw std::out_of_range("");
    }
}

bool Rom::parseRomHeader() {
    if (!((rom_[0] == 'N') &&
          (rom_[1] == 'E') &&
          (rom_[2] == 'S'))) {
        return false;
    };

    if (rom_[3] != 0x1A) {
        return false;
    }
    // TODO Add more checks
    // iNES, NES 2.0 etc.

    // Flags
    flags_6_  = rom_[6];
    flags_7_  = rom_[7];
    flags_8_  = rom_[8];
    flags_9_  = rom_[9];
    flags_10_ = rom_[10];

    prg_rom_blocks_ = rom_[4]; // 16KB units
    chr_rom_blocks_ = rom_[5]; // 8KB units

    return true;
};

void Rom::printRomInfo() {
    printf("  Format: iNES\n");
    printf("  PRG ROM size: 0x%x (%uKB)\n", prg_rom_blocks_, prg_rom_blocks_ * 16);
    printf("  CHR ROM size: 0x%x (%uKB)\n", chr_rom_blocks_, chr_rom_blocks_ * 8);
    printf("  Mapper: 0x%x\n", (flags_6_ & 0xF0) >> 4 | (flags_7_ & 0xF0));
}
