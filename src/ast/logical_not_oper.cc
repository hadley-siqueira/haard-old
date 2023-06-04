#include <sstream>
#include "ast/logical_not_oper.h"

using namespace haard;

LogicalNotOper::LogicalNotOper(Expression* expression) {
    this->kind = EXPR_LOGICAL_NOT_OPER;
    this->expression = expression;
}

LogicalNotOper::LogicalNotOper(Token& token, Expression* expression) {
    this->kind = EXPR_LOGICAL_NOT_OPER;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

LogicalNotOper::~LogicalNotOper() {
    delete expression;
}

Expression* LogicalNotOper::get_expression() const {
    return expression;
}

void LogicalNotOper::set_expression(Expression* value) {
    expression = value;
}

std::string LogicalNotOper::to_str() {
    std::stringstream ss;

    ss << "!" << expression->to_str();

    return ss.str();
}
