#include "system.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc == 2) {
    System *emulator = new System(argv[1]);
    emulator->run();
  };

  return 0;
};

