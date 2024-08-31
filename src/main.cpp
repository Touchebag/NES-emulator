#include "system.h"

int main(int argc, char *argv[]) {
  if (argc == 2) {
    System emulator;
    emulator.loadRom(argv[1]);
    emulator.run();
  };

  return 0;
};

