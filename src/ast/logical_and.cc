#include <sstream>
#include "ast/logical_and.h"

using namespace haard;

LogicalAnd::LogicalAnd(Expression* left, Expression* right) {
    this->kind = EXPR_LOGICAL_AND;
    this->left = left;
    this->right = right;
}

LogicalAnd::LogicalAnd(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_LOGICAL_AND;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

LogicalAnd::~LogicalAnd() {
    delete left;
    delete right;
}

Expression* LogicalAnd::get_left() const {
    return left;
}

void LogicalAnd::set_left(Expression* value) {
    left = value;
}

Expression* LogicalAnd::get_right() const {
    return right;
}

void LogicalAnd::set_right(Expression* value) {
    right = value;
}

std::string LogicalAnd::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " and ";
    ss << right->to_str();

    return ss.str();
}
