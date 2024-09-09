#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "memory.h"

class System {
  public:
    System();

    void loadRom(const std::string& rom_file);

    void reset();
    void run(sf::RenderWindow& window);

  private:
    Cpu cpu_;
    Ppu ppu_;
    Memory memory_;
};

