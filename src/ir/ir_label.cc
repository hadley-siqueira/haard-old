#include "ir/ir_label.h"

using namespace haard;

IRLabel::IRLabel() {
    kind = IR_LABEL;
}

std::string IRLabel::get_label() const {
    return label;
}

void IRLabel::set_label(std::string value) {
    this->label = value;
}
