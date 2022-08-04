#include <cstdint>
#include <cstdio>
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
        write32(0, 0xffabcdef);
    }

    void print_byte(uint8_t byte) {
        printf("%x\n", byte);
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

    void write32(uint64_t addr, uint32_t value) {
        mem[addr + 0] = (value >> 24) & 0x0ff;
        mem[addr + 1] = (value >> 16) & 0x0ff;
        mem[addr + 2] = (value >> 8) & 0x0ff;
        mem[addr + 3] = (value >> 0) & 0x0ff;
    }

    void dump() {
        auto i = 0;
        while (i < 10) {
            print_byte(mem[i]);
            i = i + 1;
        }
    }


};

int main() {
    auto v = HivekVM();
    v.initialize();
    v.dump();
}

