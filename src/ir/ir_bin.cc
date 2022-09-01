#include <sstream>
#include "ir/ir_bin.h"

using namespace haard;

IRBin::IRBin(int kind, IRValue* dst, IRValue* src1, IRValue* src2) {
    this->dst = dst;
    this->src1 = src1;
    this->src2 = src2;
}

IRBin::~IRBin() {

}

std::string IRBin::to_str() {
    switch (kind) {
    case IR_ADD:
        return to_str1("add");
    }

    return "unknown ir_bin";
}

std::string IRBin::to_str1(std::string op) {
    std::stringstream ss;

    ss << op << " " << dst->to_str() << ", " << src1->to_str() << ", " << src2->to_str();
    return ss.str();
}
