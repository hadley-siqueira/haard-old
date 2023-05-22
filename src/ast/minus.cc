#include <sstream>
#include "ast/minus.h"

using namespace haard;

Minus::Minus(Expression* left, Expression* right) {
    this->kind = EXPR_MINUS;
    this->left = left;
    this->right = right;
}

Minus::Minus(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_MINUS;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

Minus::~Minus() {
    delete left;
    delete right;
}

Expression* Minus::get_left() const {
    return left;
}

void Minus::set_left(Expression* value) {
    left = value;
}

Expression* Minus::get_right() const {
    return right;
}

void Minus::set_right(Expression* value) {
    right = value;
}

std::string Minus::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " - ";
    ss << right->to_str();

    return ss.str();
}
