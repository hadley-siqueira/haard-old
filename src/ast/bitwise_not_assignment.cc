#include <sstream>
#include "ast/bitwise_not_assignment.h"

using namespace haard;

BitwiseNotAssignment::BitwiseNotAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_NOT_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

BitwiseNotAssignment::BitwiseNotAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_NOT_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

BitwiseNotAssignment::~BitwiseNotAssignment() {
    delete left;
    delete right;
}

Expression* BitwiseNotAssignment::get_left() const {
    return left;
}

void BitwiseNotAssignment::set_left(Expression* value) {
    left = value;
}

Expression* BitwiseNotAssignment::get_right() const {
    return right;
}

void BitwiseNotAssignment::set_right(Expression* value) {
    right = value;
}

std::string BitwiseNotAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " ~= ";
    ss << right->to_str();

    return ss.str();
}
