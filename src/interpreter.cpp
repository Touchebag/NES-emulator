#include "../inc/interpreter.h"

#ifdef DEBUG
// Add extra stuff for debugging
#include "stdlib.h"
#include <iomanip>
#define VERBOSE
using namespace std;
#endif

Interpreter::Interpreter(Registers *registers, Cpu_ram *ram) {
  reg = registers;
  memory = ram;
};

Interpreter::~Interpreter() {
};

void Interpreter::set_pc(BYTE low, BYTE high) {
  reg->pc[0] = low;
  reg->pc[1] = high;
};

void Interpreter::inc_pc(int number) {
  for (int i = 0; i < number; i++) {
    if (reg->pc[0] == 0xFF) {
      reg->pc[1] = (reg->pc[1] + 1) % 256;
    };
    reg->pc[0] = (reg->pc[0] + 1) % 256;
  };
};

BYTE Interpreter::read_from_pc() {
  return memory->read_ram(reg->pc[0], reg->pc[1]);
};

void Interpreter::set_status_bit(Status_flag bit, bool value) {
  if (value) {
    reg->p |= bit;
  } else {
    reg->p &= ~bit;
  };
};

void Interpreter::set_negative_flag(BYTE value) {
  bool bit = (value >> 7) & 1;

  set_status_bit(NEGATIVE_FLAG, bit);
};

void Interpreter::set_zero_flag(BYTE value) {
  set_status_bit(ZERO_FLAG, (value == 0));
};

/* Main interpreter loop */
int Interpreter::execute_instruction() {
  int opcode = read_from_pc();
  int cycles = 0;

  /* Helper variables to avoid declaring new variables in switch statement
   * Also to keep track of what they are used for
   *
   * hi/lo -> Memory addresses
   * c     -> constants
   * a,x.y -> contents of registers
   * m     -> value in memory
   * tmp   -> temporary calculations
   */
  BYTE hi, lo, c, a, x, y, tmp;

  switch (opcode) {
    // BEQ {{{
    case 0xF0:
      // Cycles first to help with page crossing
      cycles = 2;

      lo = reg->pc[0];
      hi = reg->pc[1];
      inc_pc(1);
      tmp = read_from_pc();
      inc_pc(1);

      // If latest operation was zero
      if (reg->p & ZERO_FLAG) {
        // If highest bit is set (negative)
        if (tmp & 128) {
          tmp &= 127;
          // If no page crossing
          if (tmp <= lo) {
            lo -= tmp;
            cycles += 1;
          } else {
            //TODO Ugly as balls, neccessary because of underflow?
            tmp -= lo;
            lo = 256 - tmp;
            hi--;
            cycles += 2;
          };
        } else { // If positive
          // If no page crossing
          if (tmp <= (255 - lo)) {
            lo += tmp;
            cycles += 1;
          } else {
            lo = (lo + tmp) % 255;
            hi++;
            cycles += 2;
          };
        };
      set_pc(lo, hi);
      };

#ifdef VERBOSE
      cout << std::hex << opcode << " BEQ " << (int)tmp << "\n";
#endif

      break;
    // }}}
    // CMP {{{
    case 0xC9:
      inc_pc(1);
      c = read_from_pc();
      inc_pc(1);

      // Compare
      a = reg->a;
      tmp = a - c;

      // Set flags
      set_negative_flag(tmp);
      set_zero_flag(tmp);
      // 0 if c > a
      set_status_bit(CARRY_FLAG, c <= a);

      // Set cycles
      cycles = 2;

#ifdef VERBOSE
      cout << std::hex << opcode << " CMP #" << (int)c << "\n";
#endif
      break;

    // }}}
    // INX {{{
    case 0xE8:
      inc_pc(1);
      reg->x = (reg->x + 1) % 256;

      // Set flags
      set_negative_flag(reg->x);
      set_zero_flag(reg->x);

      // Number of cycles
      cycles = 2;

#ifdef VERBOSE
      cout << std::hex << opcode << " INX" << "\n";
#endif
      break;
    // }}}
    // LDA {{{
    // Immediate {{{
    case 0xA9:
      inc_pc(1);
      c = read_from_pc();
      reg->a = c;
      inc_pc(1);

      // Set status
      set_negative_flag(c);
      set_zero_flag(c);

      // Number of cycles
      cycles = 2;

#ifdef VERBOSE
      cout << std::hex << opcode << " LDA #" << (int)c << "\n";
#endif
      break; // }}}
    // Absolute, X {{{
    case 0xBD:
      // Cycles first for easier adding of potential page-crossing cycle
      cycles = 4;

      inc_pc(1);
      lo = read_from_pc();
      inc_pc(1);
      hi = read_from_pc();
      inc_pc(1);
      x = reg->x;

      // If no page crossing
      if (x < (256 - lo)) {
        lo += x;
      } else {
        lo = (lo + x) % 255;
        hi++;
        cycles++;
      };

      // Store to A
      a = memory->read_ram(lo, hi);
      reg->a = a;

      // Set status
      set_negative_flag(a);
      set_zero_flag(a);

#ifdef VERBOSE
      cout << std::hex << opcode << " LDA "
        << setfill('0') << setw(2) << (int)hi
        << setfill('0') << setw(2) << (int)lo
        << " " << (int)x << " " << (int)a << "\n";
#endif
      break;
      // }}}
      // }}}
    // LDX {{{
    // Immediate {{{
    case 0xA2:
      inc_pc(1);
      c = read_from_pc();
      reg->x = c;
      inc_pc(1);

      // Set status
      set_negative_flag(c);
      set_zero_flag(c);

      // Number of cycles
      cycles = 2;

#ifdef VERBOSE
      cout << std::hex << opcode << " LDX #" << (int)c << "\n";
#endif
      break;
      // }}}
      // }}}
    // STA {{{
    // Absolute {{{
    case 0x8D:
      inc_pc(1);
      lo = read_from_pc();
      inc_pc(1);
      hi = read_from_pc();
      inc_pc(1);

      memory->write_ram(lo, hi, reg->a);

      // Number of cycles
      cycles = 3;

#ifdef VERBOSE
      cout << std::hex << opcode << " STA "
        << setfill('0') << setw(2) << (int)hi
        << setfill('0') << setw(2) << (int)lo
        << " " << (int)reg->a << "\n";
#endif
      break;
      // }}}
      // }}}
    default:
#ifdef VERBOSE
      cout << "Unknown opcode " << std::hex << (int)opcode;
      exit(1);
#endif
      break;
  };

#ifdef DEBUG
  if (cycles == 0) {
    cout << "WARNING unfinished opcode: " << opcode << "\n";
  };
#endif

  // ppu.cycle(cycles);

  return OK;
};
