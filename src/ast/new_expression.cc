#include "ast/new_expression.h"

using namespace haard;

NewExpression::NewExpression() {
    kind = EXPR_NEW;
    args = nullptr;
    new_type = nullptr;
    array_size = nullptr;
}

NewExpression::~NewExpression() {
    delete args;
}

Type* NewExpression::get_new_type() {
    return new_type;
}

ExpressionList* NewExpression::get_arguments() {
    return args;
}

Expression* NewExpression::get_array_size() {
    return array_size;
}

void NewExpression::set_new_type(Type* type) {
    new_type = type;
}

void NewExpression::set_arguments(ExpressionList* args) {
    this->args = args;
}

void NewExpression::set_array_size(Expression *expr) {
    array_size = expr;
}

bool NewExpression::has_arguments() {
    return args != nullptr;
}

Expression* NewExpression::clone() {
    NewExpression* other = new NewExpression();

    if (new_type) {
        other->new_type = new_type->clone();
    }

    if (args) {
        other->args = (ExpressionList*) args->clone();
    }

    return other;
}
