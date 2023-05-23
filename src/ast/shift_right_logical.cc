#include <sstream>
#include "ast/shift_right_logical.h"

using namespace haard;

ShiftRightLogical::ShiftRightLogical(Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_RIGHT_LOGICAL;
    this->left = left;
    this->right = right;
}

ShiftRightLogical::ShiftRightLogical(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_RIGHT_LOGICAL;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

ShiftRightLogical::~ShiftRightLogical() {
    delete left;
    delete right;
}

Expression* ShiftRightLogical::get_left() const {
    return left;
}

void ShiftRightLogical::set_left(Expression* value) {
    left = value;
}

Expression* ShiftRightLogical::get_right() const {
    return right;
}

void ShiftRightLogical::set_right(Expression* value) {
    right = value;
}

std::string ShiftRightLogical::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " >>> ";
    ss << right->to_str();

    return ss.str();
}
