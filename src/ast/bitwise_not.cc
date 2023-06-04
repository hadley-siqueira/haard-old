#include <sstream>
#include "ast/bitwise_not.h"

using namespace haard;

BitwiseNot::BitwiseNot(Expression* expression) {
    this->kind = EXPR_BITWISE_NOT;
    this->expression = expression;
}

BitwiseNot::BitwiseNot(Token& token, Expression* expression) {
    this->kind = EXPR_BITWISE_NOT;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

BitwiseNot::~BitwiseNot() {
    delete expression;
}

Expression* BitwiseNot::get_expression() const {
    return expression;
}

void BitwiseNot::set_expression(Expression* value) {
    expression = value;
}

std::string BitwiseNot::to_str() {
    std::stringstream ss;

    ss << "~" << expression->to_str();

    return ss.str();
}
