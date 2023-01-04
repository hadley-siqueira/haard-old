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

IRValue *IRUnary::get_dst() const {
    return dst;
}

void IRUnary::set_dst(IRValue *value) {
    dst = value;
}

IRValue *IRUnary::get_src() const {
    return src;
}

void IRUnary::set_src(IRValue *value) {
    src = value;
}

