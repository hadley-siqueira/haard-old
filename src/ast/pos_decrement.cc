#include <sstream>
#include "ast/pos_decrement.h"

using namespace haard;

PosDecrement::PosDecrement(Expression* expression) {
    this->kind = EXPR_POS_DECREMENT;
    this->expression = expression;
}

PosDecrement::PosDecrement(Token& token, Expression* expression) {
    this->kind = EXPR_POS_DECREMENT;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

PosDecrement::~PosDecrement() {
    delete expression;
}

Expression* PosDecrement::get_expression() const {
    return expression;
}

void PosDecrement::set_expression(Expression* value) {
    expression = value;
}

std::string PosDecrement::to_str() {
    std::stringstream ss;

    ss << "--" << expression->to_str();

    return ss.str();
}
