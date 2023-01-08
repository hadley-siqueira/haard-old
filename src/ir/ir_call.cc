#include "ir/ir_call.h"

using namespace haard;

IRCall::IRCall() {
    set_kind(IR_CALL);
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
