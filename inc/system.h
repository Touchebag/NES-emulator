#ifndef SYSTEM
#define SYSTEM

#include <iostream>
#include <stdio.h>
#include <sys/stat.h>

#include "utility.h"
#include "cpu_ram.h"
#include "interpreter.h"

// Main system class
class System {
  // friend class Interpreter;
  public:
    /* Initialises the emulator (sets pc, loads prg rom etc)
     *
     * Returns the error code
     */
    int init(std::string);

    // The main loop of the program. Run only after init
    void run();


#ifdef DEBUG
    // When debugging ROM is public
    BYTE *current_rom;
#endif

  private:
    /*********** Classes ************/

    Cpu_ram *memory;
    Interpreter *cpu;

    /*********** Variables ************/

#ifndef DEBUG
    // The (full) currently loaded rom image
    BYTE *current_rom;
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

    /* Loads a ROM image. Calls all neccessary subroutines to load a ROM file
     *
     * Argument is the filepath
     *
     * Returns the error code of any subroutine
     */
    int load_rom_image(std::string);

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
