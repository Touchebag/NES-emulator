#ifndef SYSTEM
#define SYSTEM

#include <iostream>
#include <stdio.h>
#include <sys/stat.h>

typedef unsigned char BYTE;

// Structure for the registers in the NES architecture
struct Registers{
  BYTE pc;
  BYTE sp;
  BYTE p;
  BYTE acc;
  BYTE x;
  BYTE y;
};

// Main system class
class System {
  public:
    // Read a NES rom image and store internally
    int open_rom(std::string);

  private:
    // The (full) currently loaded rom image
    BYTE *current_rom;
    // Rom image size in bytes
    int current_rom_size;
};

#endif
