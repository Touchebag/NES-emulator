#ifndef UTILITY
#define UTILITY

#include <stdint.h>

// For ease of use
typedef uint8_t BYTE;

// Structure for the registers in the NES architecture
struct Registers {
  BYTE pc[2];
  BYTE sp;
  BYTE p;
  BYTE a;
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
enum Status_flag {
  CARRY_FLAG = 1,
  ZERO_FLAG = 2,
  INTERRUPT_FLAG = 4,
  DECIMAL_FLAG = 8,
  BREAK_FLAG = 16,
  UNUSED_FLAG = 32,
  OVERFLOW_FLAG = 64,
  NEGATIVE_FLAG = 128
};

// PRG ROM block slots in RAM
enum Block_slot {
  PRG_SLOT_LOWER,
  PRG_SLOT_UPPER
};

#endif
