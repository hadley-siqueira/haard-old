#include "ast/unary_operator.h"

using namespace haard;

UnaryOperator::UnaryOperator() {
    set_expression(nullptr);
}

UnaryOperator::~UnaryOperator() {
    delete expression;
}

Expression* UnaryOperator::get_expression() const {
    return expression;
}

void UnaryOperator::set_expression(Expression* value) {
    expression = value;
}
