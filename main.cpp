#include "chip8.h"
#include <iostream>

int main() {
    Chip8 chip8;
    chip8.initialize();

    if (!chip8.loadROM("roms/PONG.ch8")) {
        return 1;
    }

    
    for (int i = 0; i < 10; ++i) { 
        chip8.emulateCycle();
        if (chip8.drawFlag) {
            std::cout << "Drawing screen..." << std::endl;
            chip8.drawFlag = false;
        }
    }

    return 0;
}
