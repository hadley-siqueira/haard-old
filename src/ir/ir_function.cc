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

IRValue* IRFunction::get_parameter(int idx) {
    if (idx < parameters_count()) {
        return params[idx];
    }

    return nullptr;
}

void IRFunction::add_parameter(IRValue* param) {
    params.push_back(param);
}

int IRFunction::temp_count() {
    return ctx->temp_count();
}

IRValue* IRFunction::get_temp(int idx) {
    return ctx->get_temp(idx);
}
