#ifndef UTILITY
#define UTILITY

// For ease of use
typedef unsigned char BYTE;

// Structure for the registers in the NES architecture
struct Registers {
  BYTE pc;
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

#endif
