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

// TODO Very ugly. Neccessary?
void Interpreter::inc_pc(int number) {
  for (int i = 0; i < number; i++) {
    if (reg->pc[0] == 0xFF) {
      reg->pc[1] = (reg->pc[1] + 1) % 256;
    };
    reg->pc[0] = (reg->pc[0] + 1) % 256;
  };
};

int Interpreter::execute_instruction() {
  int opcode = memory->read_ram(reg->pc[0], reg->pc[1]);

  switch (opcode) {
    case 0:
      inc_pc(1);
#ifdef VERBOSE
      cout << opcode << " NOP\n";
#endif
      break;
    default:
#ifdef VERBOSE
      cout << "Unknown opcode " << opcode;
      exit(1);
#endif
      break;
  };
  return OK;
};
