#include <sstream>
#include "ir/ir_value.h"
#include "utils/string_pool.h"
#include "defs.h"

using namespace haard;

IRValue::IRValue() {
    kind = -1;
    value = nullptr;
}

IRValue::IRValue(int kind, const char* value) {
    this->kind = kind;
    this->value = value;
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
        ss << value;
        break;
    }

    return ss.str();
}
