#include "rom.h"

#include <fstream>

#include "mappers/mapper0.h"

#include "log.h"

Rom::Rom() {
    rom_ = std::vector<uint8_t>(0x8000, 0);
}

std::shared_ptr<Rom> Rom::loadRomFromFile(const std::string& filepath) {
    std::ifstream infile(filepath, std::ios_base::binary);
    if (infile.fail()) {
        throw std::invalid_argument(std::string("Could not open ROM file ") + filepath);
    }
    auto file_contents = std::vector<uint8_t>(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
    infile.close();

    auto header = RomHeader(std::vector<uint8_t>(file_contents.begin(), file_contents.begin() + 0x10));

    std::shared_ptr<Rom> rom = std::make_shared<Rom>();
    if (header.mapper == 0x00) {
        rom = std::make_shared<mappers::Mapper0>();
    }

    rom->header_ = header;
    rom->rom_ = std::vector<uint8_t>(file_contents.begin() + 0x10, file_contents.end());

    printf("ROM file %s loaded successfully\n", filepath.c_str());
    rom->header_.printHeaderInfo();

    return rom;
}

uint8_t Rom::readAddress(uint16_t address) {
    address = address & 0x7FFF;
    return rom_.at(address);
}

void Rom::writeAddress(uint16_t address, uint8_t value) {
    address = address & 0x7FFF;
    rom_.at(address) = value;
}
