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

  private:
    System();

    Cpu cpu_;
    Ppu ppu_;
    Memory memory_;
};

