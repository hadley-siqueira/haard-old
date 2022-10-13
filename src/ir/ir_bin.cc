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

std::string IRBin::to_str1(std::string op) {
    std::stringstream ss;

    ss << op << " " << dst->to_str() << ", " << src1->to_str() << ", " << src2->to_str();
    return ss.str();
}
