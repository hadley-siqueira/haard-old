#include <sstream>
#include "ast/bitwise_or.h"

using namespace haard;

BitwiseOr::BitwiseOr(Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_OR;
    this->left = left;
    this->right = right;
}

BitwiseOr::BitwiseOr(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_OR;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

BitwiseOr::~BitwiseOr() {
    delete left;
    delete right;
}

Expression* BitwiseOr::get_left() const {
    return left;
}

void BitwiseOr::set_left(Expression* value) {
    left = value;
}

Expression* BitwiseOr::get_right() const {
    return right;
}

void BitwiseOr::set_right(Expression* value) {
    right = value;
}

std::string BitwiseOr::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " | ";
    ss << right->to_str();

    return ss.str();
}
