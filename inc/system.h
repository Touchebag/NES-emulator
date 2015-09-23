#ifndef SYSTEM
#define SYSTEM

#include <iostream>
#include <stdio.h>
#include <sys/stat.h>

#include "utility.h"

// Main system class
class System {
  public:
    /* Reads a NES rom image and store in current_rom
     *
     * Argument is the filepath
     *
     * Returns the error code
     */
    int open_rom(std::string);

    /* Checks if the currently loaded file is actaully a valid NES rom image
     *
     * Returns the error code
     */
    int check_current_rom();

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
