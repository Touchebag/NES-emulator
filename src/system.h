#pragma once

#include <string>

#include "rom.h"
#include "cpu.h"
#include "memory.h"

class System {
  public:
    System();

    void loadRom(const std::string& rom_file);

    void reset();
    void run();

  private:
    Cpu cpu_;
    Memory memory_;
};

