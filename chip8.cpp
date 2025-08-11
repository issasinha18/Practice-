#include "chip8.h"
#include <fstream>
#include <iostream>
#include<vector>

void Chip8::initialize() {
    pc = 0x200; 
    opcode = 0;
    I = 0;
    delayTimer = 0;
    soundTimer = 0;

    memory.fill(0);
    V.fill(0);
    display.fill(0);


    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, 
        0x20, 0x60, 0x20, 0x20, 0x70, 
        0xF0, 0x10, 0xF0, 0x80, 0xF0, 
        0xF0, 0x10, 0xF0, 0x10, 0xF0, 
        0x90, 0x90, 0xF0, 0x10, 0x10, 
        0xF0, 0x80, 0xF0, 0x10, 0xF0, 
        0xF0, 0x80, 0xF0, 0x90, 0xF0, 
        0xF0, 0x10, 0x20, 0x40, 0x40, 
        0xF0, 0x90, 0xF0, 0x90, 0xF0, 
        0xF0, 0x90, 0xF0, 0x10, 0xF0, 
        0xF0, 0x90, 0xF0, 0x90, 0x90, 
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0, 
        0xF0, 0x80, 0xF0, 0x80, 0xF0, 
        0xF0, 0x80, 0xF0, 0x80, 0x80  
    };

    for (int i = 0; i < 80; ++i) {
        memory[i] = fontset[i];
    }
}

bool Chip8::loadROM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open ROM: " << filename << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size > (4096 - 512)) {
        std::cerr << "ROM too big to fit in memory." << std::endl;
        return false;
    }

    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    for (size_t i = 0; i < size; ++i)
        memory[0x200 + i] = static_cast<uint8_t>(buffer[i]);

    return true;
}

void Chip8::emulateCycle() {
    opcode = memory[pc] << 8 | memory[pc + 1];


    executeOpcode();


    if (delayTimer > 0) --delayTimer;
    if (soundTimer > 0) --soundTimer;
}

void Chip8::executeOpcode() {
    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {
                
                display.fill(0);
                drawFlag = true;
                pc += 2;
            }
            break;

        case 0x1000:
    
            pc = opcode & 0x0FFF;
            break;

        case 0x6000: {
            
            uint8_t x = (opcode & 0x0F00) >> 8;
            V[x] = opcode & 0x00FF;
            pc += 2;
            break;
        }

        case 0x7000: {

            uint8_t x = (opcode & 0x0F00) >> 8;
            V[x] += opcode & 0x00FF;
            pc += 2;
            break;
        }

        default:
            std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
            pc += 2;
            break;
    }
}
