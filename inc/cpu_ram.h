#ifndef CPU_RAM
#define CPU_RAM

#include <iostream>
#include "utility.h"

class Cpu_ram {
  public:
#ifdef DEBUG
    // When debugging RAM is public
    BYTE system_ram[0x0800];
    BYTE ppu_registers[0x0008];
    BYTE io_registers[0x0020];
    BYTE expansion_rom[0x1FE0];
    BYTE sram[0x2000];
    BYTE prg_rom_lower_bank[0x4000];
    BYTE prg_rom_upper_bank[0x4000];
#endif

    /* Reads an address from memory
     *
     * Arguments are the lowest byte (LSB)
     * and the highest (MSB) byte, in that order
     *
     * Returns the byte stored at location (default 0)
     */
    BYTE read_ram(BYTE address_low, BYTE address_high);

    /* Writes to an address in memory
     *
     * Arguments are the lowest byte (LSB)
     * and the highest (MSB) byte, in that order,
     * as well as the value to write
     */
    void write_ram(BYTE address_low, BYTE address_high, BYTE value);


  private:
#ifndef DEBUG
    // Pointers to all sections of system RAM
    BYTE system_ram[0x0800];
    BYTE ppu_registers[0x0008];
    BYTE io_registers[0x0020];
    BYTE expansion_rom[0x1FE0];
    BYTE sram[0x2000];
    BYTE prg_rom_lower_bank[0x4000];
    BYTE prg_rom_upper_bank[0x4000];
#endif

    // RAM addresses
    enum Ram_pointers {
      SYSTEM_RAM = 0x0000,
      PPU_REGISTERS = 0x2000,
      IO_REGISTERS = 0x4000,
      EXPANSION_ROM = 0x4020,
      SRAM = 0x6000,
      PRG_ROM_LOWER = 0x8000,
      PRG_ROM_UPPER = 0xC000,
    };

    BYTE* access_memory_byte(BYTE address_low, BYTE address_high);

};
#endif
