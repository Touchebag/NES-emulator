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
    /* Reads a NES rom image and store in current_rom
     *
     * Argument is the filepath
     *
     * Returns 0 if no errors nad -1 otherwise
     */
    int open_rom(std::string);
#ifdef DEBUG
    // When debugging ROM image is public
    BYTE *current_rom;
#endif

  private:
    // The (full) currently loaded rom image
#ifndef DEBUG
    BYTE *current_rom;
#endif
    // Rom image size in bytes
    int current_rom_size;
    // The NES cpu registers
    Registers reg;
};

#endif
