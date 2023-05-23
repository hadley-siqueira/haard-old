#include <sstream>
#include "ast/in.h"

using namespace haard;

In::In(Expression* left, Expression* right) {
    this->kind = EXPR_IN;
    this->left = left;
    this->right = right;
}

In::In(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_IN;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

In::~In() {
    delete left;
    delete right;
}

Expression* In::get_left() const {
    return left;
}

void In::set_left(Expression* value) {
    left = value;
}

Expression* In::get_right() const {
    return right;
}

void In::set_right(Expression* value) {
    right = value;
}

std::string In::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " in ";
    ss << right->to_str();

    return ss.str();
}
