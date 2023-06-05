#include <iostream>
#include "ast/expression.h"

using namespace haard;

Expression::Expression() {
    type = nullptr;
}

Expression::~Expression() {
    // delete type;
}

Type* Expression::get_type() {
    return type;
}

void Expression::set_type(Type* type) {
    this->type = type;
}
