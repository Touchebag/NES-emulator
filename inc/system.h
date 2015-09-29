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
    BYTE *system_ram[0x8000];
    BYTE *prg_rom_lower_bank[0x4000];
    BYTE *prg_rom_upper_bank[0x4000];
#endif

  private:

    /*********** Variables ************/

#ifndef DEBUG
    // The (full) currently loaded rom image
    BYTE *current_rom;
    // The system RAM
    BYTE *system_ram[0x8000];
    // With pointers to prg rom banks
    BYTE *prg_rom_lower_bank[0x4000];
    BYTE *prg_rom_upper_bank[0x4000];
#endif
    // Rom image size in bytes
    int current_rom_size;
    // The NES cpu registers
    Registers reg;

    /* --- ROM info --- */

    // PRG ROM size (# of 16KiB blocks)
    int prg_rom_blocks;
    // PRG ROM pointer
    BYTE **prg_rom;
    // CHR ROM size (# of 8KiB blocks)
    int chr_rom_blocks;
    // CHR ROM pointer
    BYTE **chr_rom;
    // True if rom has trainer data
    bool has_trainer;



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
