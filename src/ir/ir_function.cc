#include <sstream>
#include "ir/ir_function.h"

using namespace haard;

IRFunction::IRFunction() {
    kind = IR_FUNCTION;
    ctx = new IRContext();
}

IRFunction::~IRFunction() {
    delete ctx;
}

IRContext* IRFunction::get_context() {
    return ctx;
}

void IRFunction::set_name(std::string name) {
    this->name = name;
}

std::string IRFunction::to_str() {
    std::stringstream ss;

    ss << "def " << name << "(";

    for (int i = 0; i < params.size(); ++i) {
        ss << params[i]->to_str() << ", ";
    }

    ss << "):\n";
    ss << ctx->to_str();

    return ss.str();
}

std::string IRFunction::to_cpp() {
    std::stringstream ss;

    ss << "void " << name << "(";

    for (int i = 0; i < params.size(); ++i) {
        ss << params[i]->to_cpp() << ", ";
    }

    ss << ") {\n";
    ss << ctx->to_cpp();
    ss << "}\n";

    return ss.str();
}
