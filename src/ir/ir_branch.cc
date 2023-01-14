#include <sstream>
#include "ir/ir_branch.h"

using namespace haard;

IRBranch::IRBranch(int kind, IRValue* src, IRValue* label) {
    this->kind = kind;
    this->label = label;
    this->src1 = src;
    this->src2 = nullptr;
}

IRBranch::~IRBranch() {
    // FIXME
    // should be deleted from a basic block
}

IRValue *IRBranch::get_label() const {
    return label;
}

void IRBranch::set_label(IRValue *value) {
    label = value;
}

IRValue *IRBranch::get_src1() const {
    return src1;
}

void IRBranch::set_src1(IRValue *value) {
    src1 = value;
}

IRValue *IRBranch::get_src2() const {
    return src2;
}

void IRBranch::set_src2(IRValue *value) {
    src2 = value;
}
