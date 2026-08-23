#include "system.h"

#include <SFML/System.hpp>

#include "log.h"

System::System() :
        cpu_{Cpu()},
        ppu_{Ppu()},
        memory_{Memory()} {
    rom_ = std::make_shared<Rom>();

    img_.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    tex_.create(WINDOW_WIDTH, WINDOW_HEIGHT);
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

void System::run(std::shared_ptr<sf::RenderWindow> window) {
    reset();
    ppu_.advance(7 * 3);

    window_ = window;

    unsigned int cycles_current_second = 0;
    // Accounting for bootup cycles
    unsigned int cycles_since_reset = 7;

    sf::Clock clock;

    if (rom_->name() == "nestest.nes") {
        cpu_.setPc(0x00, 0xC0);
    }

    if (auto w = window_.lock()) {
        while (w->isOpen()) {
            bool run_single_instruction = false;

            sf::Event event;
            while (window->pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window->close();
                        break;
                    case sf::Event::KeyPressed:
                        switch (event.key.code) {
                            case sf::Keyboard::Escape:
                                window->close();
                                break;
                            case sf::Keyboard::R:
                                running_ = !running_;
                                break;
                            case sf::Keyboard::N:
                                run_single_instruction = true;
                                break;
                            default:
                                break;
                        }
                    default:
                        break;
                }
            }

            if (running_ || run_single_instruction) {
                try {
                    auto pc = cpu_.getRegisters().pc;
                    auto current_instruction = ParsedInstruction(pc[0], pc[1]);

                    int cycles = cpu_.executeInstruction(current_instruction);

                    if (nestest_output_) {
                        current_instruction.print(cycles_since_reset);
                    }

                    ppu_.advance(cycles * 3);

                    cycles_current_second += cycles;
                    cycles_since_reset += cycles;

                    if (clock.getElapsedTime().asMilliseconds() >= 1000) {
                        LOGD("Cycles/s %i (%.1f%%)", cycles_current_second, static_cast<float>(cycles_current_second) / 17897.73 );
                        cycles_current_second = 0;

                        LOGD("Fps %i", num_frames_);
                        num_frames_ = 0;

                        clock.restart();
                    }
                } catch (std::out_of_range& e) {
                    LOGW("Total cycles: %i", cycles_since_reset);
                    throw e;
                }
            }
        }
    } else {
        LOGE("Could not lock render window");
    }
}

void System::onVsyncTriggered() {
    num_frames_++;

    if (auto window = window_.lock()) {
        const auto& framebuffer = ppu_.getFramebuffer();
        auto it = framebuffer.begin();
        for (auto x = 0; x < WINDOW_WIDTH; x++) {
            for (auto y = 0; y < WINDOW_HEIGHT; y++) {
                if (it != framebuffer.end()) {
                    img_.setPixel(x, y, {it->r, it->g, it->b});
                    it++;
                }
            }
        }

        tex_.update(img_);
        window->clear();
        window->draw(sf::Sprite(tex_));
        window->display();
    }
}

void System::enableNestestOutput(bool enable) {
    if (enable) {
        LOGI("Enabling nestest output");
    } else {
        LOGI("Disabling nestest output");
    }
    nestest_output_ = enable;
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
