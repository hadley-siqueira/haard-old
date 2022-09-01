#include <sstream>
#include "ir/ir_unary.h"

using namespace haard;

IRUnary::IRUnary(int kind, IRValue* dst, IRValue* src) {
    this->kind = kind;
    this->dst = dst;
    this->src = src;
}

IRUnary::~IRUnary() {
    delete dst;
}

std::string IRUnary::to_str() {
    std::stringstream ss;

    switch (kind) {
    case IR_LI:
        ss << "li " << dst->to_str() << ", " << src->to_str();
        break;

    case IR_COPY:
        ss << "copy " << dst->to_str() << ", " << src->to_str();
        break;

    default:
        ss << "unknown unary ir";
        break;
    }

    return ss.str();
}
