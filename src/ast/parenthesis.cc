#include <sstream>
#include "ast/parenthesis.h"

using namespace haard;

Parenthesis::Parenthesis(Expression* expression) {
    this->kind = EXPR_PARENTHESIS;
    this->expression = expression;
}

Parenthesis::Parenthesis(Token& token, Expression* expression) {
    this->kind = EXPR_PARENTHESIS;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

Parenthesis::~Parenthesis() {
    delete expression;
}

Expression* Parenthesis::get_expression() const {
    return expression;
}

void Parenthesis::set_expression(Expression* value) {
    expression = value;
}

std::string Parenthesis::to_str() {
    std::stringstream ss;

    ss << "(" << expression->to_str() << ")";

    return ss.str();
}
