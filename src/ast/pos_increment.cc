#include <sstream>
#include "ast/pos_increment.h"

using namespace haard;

PosIncrement::PosIncrement(Expression* expression) {
    this->kind = EXPR_POS_INCREMENT;
    this->expression = expression;
}

PosIncrement::PosIncrement(Token& token, Expression* expression) {
    this->kind = EXPR_POS_INCREMENT;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

PosIncrement::~PosIncrement() {
    delete expression;
}

Expression* PosIncrement::get_expression() const {
    return expression;
}

void PosIncrement::set_expression(Expression* value) {
    expression = value;
}

std::string PosIncrement::to_str() {
    std::stringstream ss;

    ss << "++" << expression->to_str();

    return ss.str();
}
