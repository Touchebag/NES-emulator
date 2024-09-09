#include "system.h"

#include <SFML/System.hpp>

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

void System::run(sf::RenderWindow& window) {
    reset();

    while (window.isOpen()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        int cycles = cpu_.executeInstruction(memory_);
        ppu_.advance(cycles);
    };
}
