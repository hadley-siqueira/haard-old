#include <sstream>
#include "ast/logical_or_oper.h"

using namespace haard;

LogicalOrOper::LogicalOrOper(Expression* left, Expression* right) {
    this->kind = EXPR_LOGICAL_OR_OPER;
    this->left = left;
    this->right = right;
}

LogicalOrOper::LogicalOrOper(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_LOGICAL_OR_OPER;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

LogicalOrOper::~LogicalOrOper() {
    delete left;
    delete right;
}

Expression* LogicalOrOper::get_left() const {
    return left;
}

void LogicalOrOper::set_left(Expression* value) {
    left = value;
}

Expression* LogicalOrOper::get_right() const {
    return right;
}

void LogicalOrOper::set_right(Expression* value) {
    right = value;
}

std::string LogicalOrOper::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " || ";
    ss << right->to_str();

    return ss.str();
}
