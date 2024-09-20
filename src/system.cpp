#include "system.h"

#include <SFML/System.hpp>

#include "log.h"

System::System() :
    cpu_{Cpu()},
    ppu_{Ppu()},
    memory_{Memory()} {
        rom_ = std::make_shared<Rom>();
}

void System::loadRom(const std::string& rom_file) {
    rom_ = Rom::loadRomFromFile(rom_file);
}

void System::resetComponents() {
    cpu_ = Cpu();
    ppu_ = Ppu();
    memory_ = Memory();
    rom_ = std::make_shared<Rom>();
}

void System::reset() {
    uint8_t pc_low = memory_.readAddress(0xFC, 0xFF);
    uint8_t pc_high = memory_.readAddress(0xFD, 0xFF);

    cpu_.setPc(pc_low, pc_high);
}

void System::run(sf::RenderWindow& window) {
    reset();

    sf::Image img;
    img.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::Texture tex;
    tex.create(WINDOW_WIDTH, WINDOW_HEIGHT);

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

        int cycles = cpu_.executeInstruction();
        ppu_.advance(cycles);

        const auto& framebuffer = ppu_.getFramebuffer();
        auto it = framebuffer.begin();
        for (auto x = 0; x < WINDOW_WIDTH; x++) {
            for (auto y = 0; y < WINDOW_HEIGHT; y++) {
                if (it != framebuffer.end()) {
                    img.setPixel(x, y, {it->r, it->g, it->b});
                    it++;
                }
            }
        }

        tex.update(img);
        window.clear();
        window.draw(sf::Sprite(tex));
        window.display();
    };
}

System& System::getInstance() {
    static System sys;

    return sys;
}

template <>
Cpu& System::get<Cpu>() {
    return getInstance().cpu_;
}

template <>
Ppu& System::get<Ppu>() {
    return getInstance().ppu_;
}

template <>
Memory& System::get<Memory>() {
    return getInstance().memory_;
}

template <>
Rom& System::get<Rom>() {
    return *(getInstance().rom_.get());
}
