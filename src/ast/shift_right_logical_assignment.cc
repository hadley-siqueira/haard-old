#include <sstream>
#include "ast/shift_right_logical_assignment.h"

using namespace haard;

ShiftRightLogicalAssignment::ShiftRightLogicalAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_RIGHT_LOGICAL_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

ShiftRightLogicalAssignment::ShiftRightLogicalAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_RIGHT_LOGICAL_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

ShiftRightLogicalAssignment::~ShiftRightLogicalAssignment() {
    delete left;
    delete right;
}

Expression* ShiftRightLogicalAssignment::get_left() const {
    return left;
}

void ShiftRightLogicalAssignment::set_left(Expression* value) {
    left = value;
}

Expression* ShiftRightLogicalAssignment::get_right() const {
    return right;
}

void ShiftRightLogicalAssignment::set_right(Expression* value) {
    right = value;
}

std::string ShiftRightLogicalAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " >>>= ";
    ss << right->to_str();

    return ss.str();
}
