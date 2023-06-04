#include <sstream>
#include "ast/dereference.h"

using namespace haard;

Dereference::Dereference(Expression* expression) {
    this->kind = EXPR_DEREFERENCE;
    this->expression = expression;
}

Dereference::Dereference(Token& token, Expression* expression) {
    this->kind = EXPR_DEREFERENCE;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

Dereference::~Dereference() {
    delete expression;
}

Expression* Dereference::get_expression() const {
    return expression;
}

void Dereference::set_expression(Expression* value) {
    expression = value;
}

std::string Dereference::to_str() {
    std::stringstream ss;

    ss << "*" << expression->to_str();

    return ss.str();
}
