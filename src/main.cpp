#include "../inc/system.h"
#include <windows.h>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc == 2) {
    System *emulator = new System(argv[1]);
    emulator->run();
  };

  system("pause");
  return 0;
};

