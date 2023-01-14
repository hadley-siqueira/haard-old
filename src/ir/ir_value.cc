#include <iostream>
#include <sstream>
#include <cstring>

#include "defs.h"
#include "ir/ir_value.h"
#include "utils/string_pool.h"
#include "utils/utils.h"

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

uint64_t IRValue::to_u64() {
    uint64_t res = 0;

    //std::cout << "CONV = " << value << std::endl;
    switch (kind) {
    case IR_VALUE_LITERAL_BOOL:
        //if (strcmp(value, "true") == 0) {
        if (value == "true" || value == "1") {
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
