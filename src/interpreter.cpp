#include "../inc/interpreter.h"

#ifdef DEBUG
// Add extra stuff for debugging
#include "stdlib.h"
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

void Interpreter::set_negative_bit(BYTE value) {
  bool bit = (value >> 7) & 1;

  set_status_bit(NEGATIVE_FLAG, bit);
};

void Interpreter::set_zero_bit(BYTE value) {
  set_status_bit(ZERO_FLAG, (value == 0));
};

int Interpreter::execute_instruction() {
  int opcode = read_from_pc();

  switch (opcode) {
    /********  LDA  ********/
    case 0xA9: { // Immediate
      inc_pc(1);
      BYTE c = read_from_pc();
      reg->a = c;
      inc_pc(1);

      // Set status
      set_negative_bit(c);
      set_zero_bit(c);

#ifdef VERBOSE
      cout << "LDA #" << std::hex << (int)c << "\n";
#endif
      break; }
    default:
#ifdef VERBOSE
      cout << "Unknown opcode " << std::hex << opcode;
      exit(1);
#endif
      break;
  };
  return OK;
};
