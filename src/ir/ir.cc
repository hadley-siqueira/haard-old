#include "ir/ir.h"

using namespace haard;

IR::~IR() {

}

void IR::set_kind(int kind) {
    this->kind = kind;
}

int IR::get_kind() {
    return kind;
}

std::string IR::to_str() {
    return "unknown ir";
}
