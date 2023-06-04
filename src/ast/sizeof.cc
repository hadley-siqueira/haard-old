#include <sstream>
#include "ast/sizeof.h"

using namespace haard;

Sizeof::Sizeof(Expression* expression) {
    this->kind = EXPR_SIZEOF;
    this->expression = expression;
}

Sizeof::Sizeof(Token& token, Expression* expression) {
    this->kind = EXPR_SIZEOF;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

Sizeof::~Sizeof() {
    delete expression;
}

Expression* Sizeof::get_expression() const {
    return expression;
}

void Sizeof::set_expression(Expression* value) {
    expression = value;
}

std::string Sizeof::to_str() {
    std::stringstream ss;

    ss << "sizeof" << expression->to_str();

    return ss.str();
}
