#include <sstream>
#include "ast/logical_not.h"

using namespace haard;

LogicalNot::LogicalNot(Expression* expression) {
    this->kind = EXPR_LOGICAL_NOT;
    this->expression = expression;
}

LogicalNot::LogicalNot(Token& token, Expression* expression) {
    this->kind = EXPR_LOGICAL_NOT;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

LogicalNot::~LogicalNot() {
    delete expression;
}

Expression* LogicalNot::get_expression() const {
    return expression;
}

void LogicalNot::set_expression(Expression* value) {
    expression = value;
}

std::string LogicalNot::to_str() {
    std::stringstream ss;

    ss << "not " << expression->to_str();

    return ss.str();
}
