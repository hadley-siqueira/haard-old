#include "ir/ir_label.h"

using namespace haard;

IRLabel::IRLabel() {
    kind = IR_LABEL;
}

const char* IRLabel::get_label() const {
    return label;
}

void IRLabel::set_label(const char* value) {
    this->label = value;
}

int IRLabel::get_address() const
{
    return address;
}

void IRLabel::set_address(int value)
{
    address = value;
}
