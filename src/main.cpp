#include "system.h"

int main(int argc, char *argv[]) {
    try {
        if (argc == 2) {
            System emulator;
            emulator.loadRom(argv[1]);
            emulator.run();
        }
    } catch (std::exception& e) {
        exit(1);
    }

    return 0;
};

