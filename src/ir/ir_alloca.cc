#include "ir/ir_alloca.h"

using namespace haard;

IRAlloca::IRAlloca() {
    size = 0;
    align = 0;
    set_kind(IR_ALLOCA);
}

int IRAlloca::get_size() const {
    return size;
}

void IRAlloca::set_size(int value) {
    size = value;
}

int IRAlloca::get_align() const {
    return align;
}

void IRAlloca::set_align(int value) {
    align = value;
}

std::string IRAlloca::get_name() const {
    return name;
}

void IRAlloca::set_name(const std::string &value) {
    name = value;
}

IRValue *IRAlloca::get_dst() const {
    return dst;
}

void IRAlloca::set_dst(IRValue* value) {
    dst = value;
}
