#include "utils/utils.h"

uint64_t conv64(std::string value) {
    uint64_t res = 0;
    int len = value.size();

    if (len > 2) {
        if (value[0] == '0' && value[1] == 'b') {
            for (int i = 2; i < len; ++i) {
                if (value[i] == '1') {
                    res = (res << 1) | 1;
                } else if (value[i] == '0') {
                    res = (res << 1) | 0;
                }
            }

            return res;
        } else if (value[0] == '0' && value[1] == 'o') {
            for (int i = 2; i < len; ++i) {
                if (value[i] >= '0' && value[i] <= '7') {
                    res = (res << 3) | (value[i] - '0');
                }
            }

            return res;
        } else if (value[0] == '0' && value[1] == 'x') {
            for (int i = 2; i < len; ++i) {
                if (value[i] >= '0' && value[i] <= '9') {
                    res = (res << 4) | (value[i] - '0');
                } else if (value[i] >= 'a' && value[i] <= 'f') {
                    res = (res << 4) | (value[i] - 'a' + 10);
                } else if (value[i] >= 'A' && value[i] <= 'F') {
                    res = (res << 4) | (value[i] - 'A' + 10);
                }
            }

            return res;
        }
    }

    for (int i = 0; i < len; ++i) {
        if (value[i] >= 0 && value[i] <= '9') {
            res = res * 10 + value[i] - '0';
        }
    }

    return res;
}

uint64_t char2u64(std::string value) {
    uint64_t res = 0;

    if (value[1] == '\\') {
        if (value[2] == 'n') {
            res = '\n';
        } else if (value[2] == 't') {
            res = '\t';
        } else if (value[2] == '\\') {
            res = '\\';
        } else if (value[2] == '\'') {
            res = '\'';
        } else if (value[2] == '0') {
            res = '\0';
        }
    } else {
        res = value[1];
    }

    return res;
}

char get_escaped(char n) {
    switch (n) {
    case 'n':
        return '\n';

    case 't':
        return '\t';

    case '\\':
        return '\\';

    case '\'':
        return '\'';

    case '0':
        return '\0';
    }

    return n;
}
