#include "ast/unary_operator.h"

using namespace haard;

UnaryOperator::UnaryOperator(Expression* expression) {
    this->kind = EXPR_ADDRESS_OF;
    this->expression = expression;
}

UnaryOperator::UnaryOperator(Token& token, Expression* expression) {
    this->kind = EXPR_ADDRESS_OF;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
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
