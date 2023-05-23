#include <sstream>
#include "ast/logical_or.h"

using namespace haard;

LogicalOr::LogicalOr(Expression* left, Expression* right) {
    this->kind = EXPR_LOGICAL_OR;
    this->left = left;
    this->right = right;
}

LogicalOr::LogicalOr(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_LOGICAL_OR;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

LogicalOr::~LogicalOr() {
    delete left;
    delete right;
}

Expression* LogicalOr::get_left() const {
    return left;
}

void LogicalOr::set_left(Expression* value) {
    left = value;
}

Expression* LogicalOr::get_right() const {
    return right;
}

void LogicalOr::set_right(Expression* value) {
    right = value;
}

std::string LogicalOr::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " or ";
    ss << right->to_str();

    return ss.str();
}
