#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

int read_int(std::string msg) {
    auto i = 0;
    std::cout << msg;
    std::cin >> i;
    return i;
}

void print(std::string msg) {
    std::cout << msg << '\n';
}

int main() {
    print("Hello, world!");
    return 0;
}

class Window {
public:
    int x;
    int y;
    int* pixels;

    void set_x(int x_) {
        auto x = x_;
        if (x > 0) {
            x = x + 1;
        }
        else if (x == 0) {
            x = 1;
        }
        else {
            x = 0;
        }
    }

    void move(int nx, int ny) {
        x = nx;
        auto y = ny;
        x = read_int("Digite n: ");
    }


};

