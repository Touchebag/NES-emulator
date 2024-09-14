#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "memory.h"

class System {
  public:
    static System& getInstance();

    template <typename T>
    static T& get();

    void loadRom(const std::string& rom_file);

    void reset();
    void resetComponents();
    void run(sf::RenderWindow& window);

    static const int WINDOW_WIDTH  = 256;
    static const int WINDOW_HEIGHT = 244;
  private:
    System();

    Cpu cpu_;
    Ppu ppu_;
    Memory memory_;
};

