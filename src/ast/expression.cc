#include "ast/expression.h"

using namespace haard;

Expression::Expression() {
    type = nullptr;
}

Expression::~Expression() {
    // delete type;
}

int Expression::get_kind() {
    return kind;
}

Type* Expression::get_type() {
    return type;
}

void Expression::set_kind(int kind) {
    this->kind = kind;
}

void Expression::set_type(Type* type) {
    this->type = type;
}


