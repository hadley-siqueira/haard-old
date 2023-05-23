#include <sstream>
#include "ast/bitwise_xor_assignment.h"

using namespace haard;

BitwiseXorAssignment::BitwiseXorAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_XOR_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

BitwiseXorAssignment::BitwiseXorAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_XOR_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

BitwiseXorAssignment::~BitwiseXorAssignment() {
    delete left;
    delete right;
}

Expression* BitwiseXorAssignment::get_left() const {
    return left;
}

void BitwiseXorAssignment::set_left(Expression* value) {
    left = value;
}

Expression* BitwiseXorAssignment::get_right() const {
    return right;
}

void BitwiseXorAssignment::set_right(Expression* value) {
    right = value;
}

std::string BitwiseXorAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " ^= ";
    ss << right->to_str();

    return ss.str();
}
