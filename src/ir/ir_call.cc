#include "ir/ir_call.h"

using namespace haard;

IRCall::IRCall() {
    set_kind(IR_CALL);
    dst = nullptr;
}

void IRCall::add_argument(IRValue* value) {
    args.push_back(value);
}

int IRCall::arguments_count() {
    return args.size();
}

IRValue* IRCall::get_argument(int idx) {
    if (idx < arguments_count()) {
        return args[idx];
    }

    return nullptr;
}

std::string IRCall::get_name() const {
    return name;
}

void IRCall::set_name(const std::string& value) {
    name = value;
}

IRValue* IRCall::get_dst() const {
    return dst;
}

void IRCall::set_dst(IRValue* value) {
    dst = value;
}
