#include "system.h"

#include <SFML/Graphics.hpp>

int main(int argc, char *argv[]) {
    try {
        if (argc == 2) {
            System emulator;
            emulator.loadRom(argv[1]);

            sf::RenderWindow window(sf::VideoMode(256, 244), "NES");
            emulator.run(window);
        }
    } catch (std::exception& e) {
        exit(1);
    }

    return 0;
};

