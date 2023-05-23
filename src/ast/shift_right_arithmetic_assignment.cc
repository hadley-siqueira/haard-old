#include <sstream>
#include "ast/shift_right_arithmetic_assignment.h"

using namespace haard;

ShiftRightArithmeticAssignment::ShiftRightArithmeticAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_RIGHT_ARITHMETIC_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

ShiftRightArithmeticAssignment::ShiftRightArithmeticAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_RIGHT_ARITHMETIC_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

ShiftRightArithmeticAssignment::~ShiftRightArithmeticAssignment() {
    delete left;
    delete right;
}

Expression* ShiftRightArithmeticAssignment::get_left() const {
    return left;
}

void ShiftRightArithmeticAssignment::set_left(Expression* value) {
    left = value;
}

Expression* ShiftRightArithmeticAssignment::get_right() const {
    return right;
}

void ShiftRightArithmeticAssignment::set_right(Expression* value) {
    right = value;
}

std::string ShiftRightArithmeticAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " >>= ";
    ss << right->to_str();

    return ss.str();
}
