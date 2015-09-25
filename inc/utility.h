#ifndef UTILITY
#define UTILITY

// For ease of use
typedef unsigned char BYTE;

// Structure for the registers in the NES architecture
struct Registers {
  BYTE pc[2];
  BYTE sp;
  BYTE p;
  BYTE acc;
  BYTE x;
  BYTE y;
};

// General error codes
enum Error_code {
  OK,
  CANNOT_OPEN_FILE,
  BAD_ROM_FILE
};

// CPU status flags
enum Status_flags {
  CARRY_FLAG = 1,
  ZERO_FLAG = 2,
  INTERRUPT_FLAG = 4,
  DECIMAL_FLAG = 8,
  BREAK_FLAG = 16,
  OVERFLOW_FLAG = 64,
  NEGATIVE_FLAG = 128
};

// RAM addresses
enum Ram_pointers {
  ZERO_PAGE = 0x0000,
  STACK = 0x0100,
  SYSTEM_RAM = 0x0200,
  IO_REGISTERS = 0x2000,
  EXPANSION_ROM = 0x4020,
  SRAM = 0x6000,
  PRG_ROM_LOWER = 0x8000,
  PRG_ROM_UPPER = 0xC000
};

#endif
