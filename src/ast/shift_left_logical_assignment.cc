#include <sstream>
#include "ast/shift_left_logical_assignment.h"

using namespace haard;

ShiftLeftLogicalAssignment::ShiftLeftLogicalAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_LEFT_LOGICAL_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

ShiftLeftLogicalAssignment::ShiftLeftLogicalAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_LEFT_LOGICAL_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

ShiftLeftLogicalAssignment::~ShiftLeftLogicalAssignment() {
    delete left;
    delete right;
}

Expression* ShiftLeftLogicalAssignment::get_left() const {
    return left;
}

void ShiftLeftLogicalAssignment::set_left(Expression* value) {
    left = value;
}

Expression* ShiftLeftLogicalAssignment::get_right() const {
    return right;
}

void ShiftLeftLogicalAssignment::set_right(Expression* value) {
    right = value;
}

std::string ShiftLeftLogicalAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " <<= ";
    ss << right->to_str();

    return ss.str();
}
