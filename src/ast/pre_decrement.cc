#include <sstream>
#include "ast/pre_decrement.h"

using namespace haard;

PreDecrement::PreDecrement(Expression* expression) {
    this->kind = EXPR_PRE_DECREMENT;
    this->expression = expression;
}

PreDecrement::PreDecrement(Token& token, Expression* expression) {
    this->kind = EXPR_PRE_DECREMENT;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

PreDecrement::~PreDecrement() {
    delete expression;
}

Expression* PreDecrement::get_expression() const {
    return expression;
}

void PreDecrement::set_expression(Expression* value) {
    expression = value;
}

std::string PreDecrement::to_str() {
    std::stringstream ss;

    ss << "--" << expression->to_str();

    return ss.str();
}
