#include <sstream>
#include "ast/bitwise_or_assignment.h"

using namespace haard;

BitwiseOrAssignment::BitwiseOrAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_OR_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

BitwiseOrAssignment::BitwiseOrAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_OR_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

BitwiseOrAssignment::~BitwiseOrAssignment() {
    delete left;
    delete right;
}

Expression* BitwiseOrAssignment::get_left() const {
    return left;
}

void BitwiseOrAssignment::set_left(Expression* value) {
    left = value;
}

Expression* BitwiseOrAssignment::get_right() const {
    return right;
}

void BitwiseOrAssignment::set_right(Expression* value) {
    right = value;
}

std::string BitwiseOrAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " |= ";
    ss << right->to_str();

    return ss.str();
}
