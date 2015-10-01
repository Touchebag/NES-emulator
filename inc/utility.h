#ifndef UTILITY
#define UTILITY

// For ease of use
typedef uint8_t BYTE;

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

#endif
