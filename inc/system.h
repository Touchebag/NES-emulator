#ifndef SYSTEM
#define SYSTEM

#include <iostream>
#include <stdio.h>
#include <sys/stat.h>

#include "utility.h"

// Main system class
class System {
  public:
    /* Loads a ROM image
     *
     * Argument is the filepath
     *
     * Returns the error code of any subroutine
     */
    int load_rom_image(std::string);


#ifdef DEBUG
    // When debugging ROM and memory is public
    BYTE *current_rom;
    BYTE *system_ram[65536];
#endif

  private:

    /*********** Variables ************/

#ifndef DEBUG
    // The (full) currently loaded rom image
    BYTE *current_rom;
    // The system RAM
    BYTE *system_ram[65536];
#endif
    // Rom image size in bytes
    int current_rom_size;
    // The NES cpu registers
    Registers reg;
    // PRG ROM size (# of 16KiB blocks)
    int prg_rom_blocks;
    // CHR ROM size (# of 8KiB blocks)
    int chr_rom_blocks;


    /*********** Functions ***********/

    /* Reads a NES rom image and store in current_rom
     *
     * Argument is the filepath
     *
     * Returns the error code
     */
    int open_rom(std::string);

    /* Checks if the currently loaded file is actually a valid NES rom image
     *
     * Returns the error code
     */
    int check_current_rom();

    /* Reads data such as PRG/CHR ROM size from the current rom
     *
     * Returns the error code
     */
    int parse_current_rom();
};

#endif
