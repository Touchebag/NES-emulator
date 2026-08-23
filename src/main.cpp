#include "system.h"

#include <SFML/Graphics.hpp>

bool parseCommandLineArgs(int argc, char *argv[]) {
    using namespace std::literals;
    auto &system = System::getInstance();
    bool rom_loaded = false;

    // Skip first argument (name of binary)
    for (auto i = 1; i < argc; i++) {
        auto current_arg = argv[i];

        if (current_arg == "--nestest"sv) {
            system.enableNestestOutput(true);
        } else if (current_arg == "--nestest-output-file"sv) {
            if (i + 1 < argc) {
                system.openNestestOutputFile(argv[i + 1]);
                i++;
            }
        } else {
            if (!rom_loaded) {
                system.loadRom(current_arg);
                rom_loaded = true;
            } else {
                LOGE("Multiple ROMs specified");
                return false;
            }
        }
    }

    if (!rom_loaded) {
        LOGW("No ROM loaded");
    }

    return true;
}

int main(int argc, char *argv[]) {
    if (!parseCommandLineArgs(argc, argv)) {
        LOGE("Failed to parse command line argument, exiting");
        return 1;
    }

    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(System::WINDOW_WIDTH, System::WINDOW_HEIGHT), "NES");

    System::getInstance().run(window);

    return 0;
};

