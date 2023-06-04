#include <sstream>
#include "ast/unary_minus.h"

using namespace haard;

UnaryMinus::UnaryMinus(Expression* expression) {
    this->kind = EXPR_UNARY_MINUS;
    this->expression = expression;
}

UnaryMinus::UnaryMinus(Token& token, Expression* expression) {
    this->kind = EXPR_UNARY_MINUS;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

UnaryMinus::~UnaryMinus() {
    delete expression;
}

Expression* UnaryMinus::get_expression() const {
    return expression;
}

void UnaryMinus::set_expression(Expression* value) {
    expression = value;
}

std::string UnaryMinus::to_str() {
    std::stringstream ss;

    ss << "-" << expression->to_str();

    return ss.str();
}
