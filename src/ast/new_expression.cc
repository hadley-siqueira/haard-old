#include "ast/new_expression.h"

using namespace haard;

NewExpression::NewExpression() {
    kind = EXPR_NEW;
    args = nullptr;
    new_type = nullptr;
}

NewExpression::~NewExpression() {
    delete args;
    delete new_type;
}

Type* NewExpression::get_new_type() {
    return new_type;
}

ExpressionList* NewExpression::get_arguments() {
    return args;
}

void NewExpression::set_new_type(Type* type) {
    new_type = type;
}

void NewExpression::set_arguments(ExpressionList* args) {
    this->args = args;
}

bool NewExpression::has_arguments() {
    return args != nullptr;
}
