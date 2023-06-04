#include <sstream>
#include "ast/unary_plus.h"

using namespace haard;

UnaryPlus::UnaryPlus(Expression* expression) {
    this->kind = EXPR_UNARY_PLUS;
    this->expression = expression;
}

UnaryPlus::UnaryPlus(Token& token, Expression* expression) {
    this->kind = EXPR_UNARY_PLUS;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

UnaryPlus::~UnaryPlus() {
    delete expression;
}

Expression* UnaryPlus::get_expression() const {
    return expression;
}

void UnaryPlus::set_expression(Expression* value) {
    expression = value;
}

std::string UnaryPlus::to_str() {
    std::stringstream ss;

    ss << "+" << expression->to_str();

    return ss.str();
}
