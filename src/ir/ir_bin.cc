#include <sstream>
#include "ir/ir_bin.h"

using namespace haard;

IRBin::IRBin(int kind, IRValue* dst, IRValue* src1, IRValue* src2) {
    this->kind = kind;
    this->dst = dst;
    this->src1 = src1;
    this->src2 = src2;
}

IRBin::~IRBin() {
    // FIXME
    // should be deleted from a basic block
}

std::string IRBin::to_str() {
    switch (kind) {
    case IR_ADD:
        return to_str1("add");

    case IR_SUB:
        return to_str1("sub");

    case IR_MUL:
        return to_str1("mul");

    case IR_DIV:
        return to_str1("div");

    }

    return "unknown ir_bin";
}

std::string IRBin::to_cpp() {
    switch (kind) {
    case IR_ADD:
        return to_cpp1("+");

    case IR_SUB:
        return to_cpp1("-");

    case IR_MUL:
        return to_cpp1("*");

    case IR_DIV:
        return to_cpp1("/");
    }

    return "unknown ir_bin cpp";
}

std::string IRBin::to_str1(std::string op) {
    std::stringstream ss;

    ss << op << " " << dst->to_str() << ", " << src1->to_str() << ", " << src2->to_str();
    return ss.str();
}

std::string IRBin::to_cpp1(std::string op) {
    std::stringstream ss;

    ss << "u64 " << dst->to_cpp() << " = " << src1->to_cpp() << " " << op << " " << src2->to_cpp() << ";";
    return ss.str();
}
