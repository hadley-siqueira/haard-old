#include <sstream>
#include "ast/pre_increment.h"

using namespace haard;

PreIncrement::PreIncrement(Expression* expression) {
    this->kind = EXPR_PRE_INCREMENT;
    this->expression = expression;
}

PreIncrement::PreIncrement(Token& token, Expression* expression) {
    this->kind = EXPR_PRE_INCREMENT;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

PreIncrement::~PreIncrement() {
    delete expression;
}

Expression* PreIncrement::get_expression() const {
    return expression;
}

void PreIncrement::set_expression(Expression* value) {
    expression = value;
}

std::string PreIncrement::to_str() {
    std::stringstream ss;

    ss << "++" << expression->to_str();

    return ss.str();
}
