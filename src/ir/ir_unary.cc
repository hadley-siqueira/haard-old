#include <sstream>
#include "ir/ir_unary.h"

using namespace haard;

IRUnary::IRUnary(int kind, IRValue* dst, IRValue* src) {
    this->kind = kind;
    this->dst = dst;
    this->src = src;
}

IRUnary::~IRUnary() {
    // FIXME
    // should be deleted from a basic block
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

    case IR_FRAME:
        ss << "frame " << dst->to_str() << ", " << src->to_str();
        break;

    case IR_LOAD:
        ss << "load " << dst->to_str() << ", " << src->to_str();
        break;

    case IR_STORE:
        ss << "store " << dst->to_str() << ", " << src->to_str();
        break;

    default:
        ss << "unknown unary ir";
        break;
    }

    return ss.str();
}

std::string IRUnary::to_cpp() {
    std::stringstream ss;

    switch (kind) {
    case IR_LI:
        ss << "u64 " << dst->to_cpp() << " = " << src->to_cpp() << ";";
        break;

    case IR_COPY:
        ss << "copy " << dst->to_cpp() << ", " << src->to_cpp();
        break;

    case IR_FRAME:
        ss << "frame " << dst->to_cpp() << ", " << src->to_cpp();
        break;

    case IR_LOAD:
        ss << "u64 " << dst->to_cpp() << " = load64(" << src->to_cpp() << ");";
        break;

    case IR_STORE:
        ss << "store64(" << dst->to_cpp() << ", " << src->to_cpp() << ");";
        break;

    default:
        ss << "unknown unary ir";
        break;
    }

    return ss.str();
}
