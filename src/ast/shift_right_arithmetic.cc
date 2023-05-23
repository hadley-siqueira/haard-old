#include <sstream>
#include "ast/shift_right_arithmetic.h"

using namespace haard;

ShiftRightArithmetic::ShiftRightArithmetic(Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_RIGHT_ARITHMETIC;
    this->left = left;
    this->right = right;
}

ShiftRightArithmetic::ShiftRightArithmetic(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_SHIFT_RIGHT_ARITHMETIC;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

ShiftRightArithmetic::~ShiftRightArithmetic() {
    delete left;
    delete right;
}

Expression* ShiftRightArithmetic::get_left() const {
    return left;
}

void ShiftRightArithmetic::set_left(Expression* value) {
    left = value;
}

Expression* ShiftRightArithmetic::get_right() const {
    return right;
}

void ShiftRightArithmetic::set_right(Expression* value) {
    right = value;
}

std::string ShiftRightArithmetic::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " >> ";
    ss << right->to_str();

    return ss.str();
}
