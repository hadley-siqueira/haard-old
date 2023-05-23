#include <sstream>
#include "ast/bitwise_xor.h"

using namespace haard;

BitwiseXor::BitwiseXor(Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_XOR;
    this->left = left;
    this->right = right;
}

BitwiseXor::BitwiseXor(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_XOR;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

BitwiseXor::~BitwiseXor() {
    delete left;
    delete right;
}

Expression* BitwiseXor::get_left() const {
    return left;
}

void BitwiseXor::set_left(Expression* value) {
    left = value;
}

Expression* BitwiseXor::get_right() const {
    return right;
}

void BitwiseXor::set_right(Expression* value) {
    right = value;
}

std::string BitwiseXor::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " ^ ";
    ss << right->to_str();

    return ss.str();
}
