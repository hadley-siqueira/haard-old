#include <iostream>
#include <sstream>
#include <cstring>
#include "ir/ir_value.h"
#include "utils/string_pool.h"
#include "defs.h"

using namespace haard;

IRValue::IRValue() {
    kind = -1;
    value = nullptr;
}

IRValue::IRValue(int kind, std::string& name) {
    this->kind = kind;
    this->value = name;
}

IRValue::IRValue(int kind, int tmp) {
    std::stringstream ss;

    ss << tmp;
    this->kind = kind;
    this->value = StringPool::get(ss.str());
}

std::string IRValue::to_str() {
    std::stringstream ss;

    switch (kind) {
    case IR_VALUE_VAR:
        ss << '@' << value;
        break;

    case IR_VALUE_TEMP:
        ss << '%' << value;
        break;

    case IR_VALUE_LITERAL_BOOL:
    case IR_VALUE_LITERAL_INTEGER:
    case IR_VALUE_LITERAL_FLOAT:
    case IR_VALUE_LITERAL_DOUBLE:
    case IR_VALUE_LITERAL_CHAR:
    case IR_VALUE_LITERAL_STRING:
    case IR_VALUE_LITERAL_SYMBOL:
    case IR_VALUE_LABEL:
        ss << value;
        break;
    }

    return ss.str();
}

int IRValue::get_kind() const {
    return kind;
}

void IRValue::set_kind(int value) {
    kind = value;
}

std::string IRValue::get_value() const {
    return value;
}

void IRValue::set_value(const char* value) {
    value = value;
}

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
        }
    } else {
        res = value[1];
    }

    return res;
}

uint64_t IRValue::to_u64() {
    uint64_t res = 0;

    //std::cout << "CONV = " << value << std::endl;
    switch (kind) {
    case IR_VALUE_LITERAL_BOOL:
        //if (strcmp(value, "true") == 0) {
        if (value == "true") {
            res = 1;
        }
        break;

    case IR_VALUE_LITERAL_CHAR:
        res = char2u64(value);
        break;

    case IR_VALUE_LITERAL_INTEGER:
        res = conv64(value);
        break;

    default:
        break;
    }

    return res;
}
