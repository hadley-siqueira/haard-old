#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

class HivekVM {
public:
    uint64_t ip;
    uint64_t regs;
    uint8_t* mem;

    void initialize() {
        ip = 0;
        mem = new uint8_t[1024 * 1024 * 8];
    }

    uint64_t read32(uint64_t addr) {
        uint64_t v = 0;
        uint64_t k;
        v = (v << 8) | mem[addr + 0];
        v = (v << 8) | mem[addr + 1];
        v = (v << 8) | mem[addr + 2];
        v = (v << 8) | mem[addr + 3];
        return v;
    }


};

