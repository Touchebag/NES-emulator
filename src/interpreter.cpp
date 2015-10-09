#include "../inc/interpreter.h"

Interpreter::Interpreter(Registers *registers, Cpu_ram *ram) {
  reg = registers;
  memory = ram;
};

void Interpreter::set_pc(BYTE low, BYTE high) {
  reg->pc[0] = low;
  reg->pc[1] = high;
  std::cout << (int)reg->pc[1] << (int)reg->pc[0] << "\n";
};

// TODO Very ugly. Neccessary?
void Interpreter::inc_pc(int number) {
  for (int i = 0; i < number; i++) {
    if (reg->pc[0] == 0xFF) {
      reg->pc[0] = 0x00;
      if (reg->pc[1] == 0xFF) {
        reg->pc[1] = 0x00;
      } else {
        reg->pc[1]++;
      };
    } else {
      reg->pc[0]++;
    };
  };

  std::cout << (int)reg->pc[1] << (int)reg->pc[0] << "\n";

};

Interpreter::~Interpreter() {
};

int Interpreter::execute_instruction() {
  return OK;
};
