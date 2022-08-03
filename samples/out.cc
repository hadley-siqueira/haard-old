#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

class Window {
public:
    int x;
    int y;
    int* pixels;

    void set_x(int x_) {
        x = x_;
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
        y = ny;
    }


};

