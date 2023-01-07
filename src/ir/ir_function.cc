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

std::string IRFunction::get_name() const {
    return name;
}

int IRFunction::parameters_count() {
    return params.size();
}

void IRFunction::add_parameter(IRValue* param) {
    params.push_back(param);
}
