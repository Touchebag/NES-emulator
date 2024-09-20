#include "rom_header.h"

#include <stdexcept>

RomHeader::RomHeader(std::vector<uint8_t> header) {
    if (header.empty()) {
        // Default construct empty header
        return;
    }

    if (header.size() != 0x10) {
        throw std::invalid_argument("Failed to parse rom: Invalid header size");
    }

    if (!((header[0] == 'N') &&
          (header[1] == 'E') &&
          (header[2] == 'S') &&
          (header[3] == 0x1A))) {
        throw std::invalid_argument("Failed to parse rom: Missing \"NES\" magic bytes");
    }

    // TODO Add more checks
    // iNES, NES 2.0 etc.

    // Flags
    flags_6  = header[6];
    flags_7  = header[7];
    flags_8  = header[8];
    flags_9  = header[9];
    flags_10 = header[10];

    prg_rom_blocks = header[4]; // 16KB units
    chr_rom_blocks = header[5]; // 8KB units
                                //
    mapper = (flags_6 & 0xF0) >> 4 | (flags_7 & 0xF0);
}

void RomHeader::printHeaderInfo() {
    printf("  Format: iNES\n");
    printf("  PRG ROM size: 0x%x (%uKB)\n", prg_rom_blocks, prg_rom_blocks * 16);
    printf("  CHR ROM size: 0x%x (%uKB)\n", chr_rom_blocks, chr_rom_blocks * 8);
    printf("  Mapper: 0x%x\n", mapper);
}
