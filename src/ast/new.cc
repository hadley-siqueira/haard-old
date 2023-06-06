#include "ast/new.h"

using namespace haard;

New::New() {
    set_kind(EXPR_NEW);
    args = nullptr;
    new_type = nullptr;
}

New::~New() {
    delete args;
}

Type* New::get_new_type() {
    return new_type;
}

ExpressionList* New::get_arguments() {
    return args;
}

void New::set_new_type(Type* type) {
    new_type = type;
}

void New::set_arguments(ExpressionList* args) {
    this->args = args;
}

bool New::has_arguments() {
    return args != nullptr;
}
