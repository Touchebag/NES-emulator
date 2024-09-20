#include "system.h"

#include <SFML/Graphics.hpp>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        System::getInstance().loadRom(argv[1]);

        sf::RenderWindow window(sf::VideoMode(System::WINDOW_WIDTH, System::WINDOW_HEIGHT), "NES");
        System::getInstance().run(window);
    }

    return 0;
};

