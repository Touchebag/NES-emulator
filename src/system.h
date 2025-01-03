#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

#include "rom.h"
#include "cpu/cpu.h"
#include "ppu.h"
#include "rom.h"
#include "memory.h"

class System {
  public:
    static System& getInstance();

    template <typename T>
    static T& get();

    void loadRom(const std::string& rom_file);

    void reset();
    void resetComponents();
    void run(std::shared_ptr<sf::RenderWindow> window);

    static const int WINDOW_WIDTH  = 256;
    static const int WINDOW_HEIGHT = 244;

    void onVsyncTriggered();

  private:
    System();

    Cpu cpu_;
    Ppu ppu_;
    Memory memory_;
    std::shared_ptr<Rom> rom_;

    unsigned int num_frames_ = 0;

    // Render stuff
    std::weak_ptr<sf::RenderWindow> window_;
    sf::Image img_;
    sf::Texture tex_;
};

