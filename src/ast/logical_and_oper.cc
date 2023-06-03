#include <sstream>
#include "ast/logical_and_oper.h"

using namespace haard;

LogicalAndOper::LogicalAndOper(Expression* left, Expression* right) {
    this->kind = EXPR_LOGICAL_AND_OPER;
    this->left = left;
    this->right = right;
}

LogicalAndOper::LogicalAndOper(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_LOGICAL_AND_OPER;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

LogicalAndOper::~LogicalAndOper() {
    delete left;
    delete right;
}

Expression* LogicalAndOper::get_left() const {
    return left;
}

void LogicalAndOper::set_left(Expression* value) {
    left = value;
}

Expression* LogicalAndOper::get_right() const {
    return right;
}

void LogicalAndOper::set_right(Expression* value) {
    right = value;
}

std::string LogicalAndOper::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " && ";
    ss << right->to_str();

    return ss.str();
}
