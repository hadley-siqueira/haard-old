#include <sstream>
#include "ast/bitwise_and.h"

using namespace haard;

BitwiseAnd::BitwiseAnd(Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_AND;
    this->left = left;
    this->right = right;
}

BitwiseAnd::BitwiseAnd(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_AND;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

BitwiseAnd::~BitwiseAnd() {
    delete left;
    delete right;
}

Expression* BitwiseAnd::get_left() const {
    return left;
}

void BitwiseAnd::set_left(Expression* value) {
    left = value;
}

Expression* BitwiseAnd::get_right() const {
    return right;
}

void BitwiseAnd::set_right(Expression* value) {
    right = value;
}

std::string BitwiseAnd::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " & ";
    ss << right->to_str();

    return ss.str();
}
