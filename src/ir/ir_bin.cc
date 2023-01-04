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

IRValue *IRBin::get_dst() const {
    return dst;
}

void IRBin::set_dst(IRValue *value) {
    dst = value;
}

IRValue *IRBin::get_src1() const {
    return src1;
}

void IRBin::set_src1(IRValue *value) {
    src1 = value;
}

IRValue *IRBin::get_src2() const {
    return src2;
}

void IRBin::set_src2(IRValue *value) {
    src2 = value;
}
