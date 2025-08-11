#pragma once
#include <array>
#include <cstdint>
#include <string>

class Chip8 {
public:
    void initialize();                
    bool loadROM(const std::string& filename);  
    void emulateCycle();             

    bool drawFlag = false;
    std::array<uint8_t, 64 * 32> display{}; 

private:
    uint16_t opcode;                 
    std::array<uint8_t, 4096> memory{}; 
    std::array<uint8_t, 16> V{};     
    uint16_t I = 0;                  
    uint16_t pc = 0x200;             
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;

    void executeOpcode();         
};
