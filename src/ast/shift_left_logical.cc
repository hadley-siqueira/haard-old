#include <sstream>
#include "ast/shift_left_logical.h"

using namespace haard;

ShiftLeftLogical::ShiftLeftLogical(Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_LEFT_LOGICAL;
    this->left = left;
    this->right = right;
}

ShiftLeftLogical::ShiftLeftLogical(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_LEFT_LOGICAL;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

ShiftLeftLogical::~ShiftLeftLogical() {
    delete left;
    delete right;
}

Expression* ShiftLeftLogical::get_left() const {
    return left;
}

void ShiftLeftLogical::set_left(Expression* value) {
    left = value;
}

Expression* ShiftLeftLogical::get_right() const {
    return right;
}

void ShiftLeftLogical::set_right(Expression* value) {
    right = value;
}

std::string ShiftLeftLogical::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " << ";
    ss << right->to_str();

    return ss.str();
}
