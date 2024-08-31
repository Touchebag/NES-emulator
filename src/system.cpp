#include "system.h"

System::System() :
    cpu_{Cpu()},
    memory_{Memory()} {
}

void System::loadRom(const std::string& rom_file) {
    memory_.loadRom(Rom::loadRomFromFile(rom_file));
}

void System::reset() {
    uint8_t pc_low = memory_.readAddress(0xFC, 0xFF);
    uint8_t pc_high = memory_.readAddress(0xFD, 0xFF);

    cpu_.setPc(pc_low, pc_high);
}

void System::run() {
    reset();

    while (true) {
        cpu_.executeInstruction(memory_);
    };
}
