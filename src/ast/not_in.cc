#include <sstream>
#include "ast/not_in.h"

using namespace haard;

NotIn::NotIn(Expression* left, Expression* right) {
    this->kind = EXPR_NOT_IN;
    this->left = left;
    this->right = right;
}

NotIn::NotIn(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_NOT_IN;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

NotIn::~NotIn() {
    delete left;
    delete right;
}

Expression* NotIn::get_left() const {
    return left;
}

void NotIn::set_left(Expression* value) {
    left = value;
}

Expression* NotIn::get_right() const {
    return right;
}

void NotIn::set_right(Expression* value) {
    right = value;
}

std::string NotIn::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " not in ";
    ss << right->to_str();

    return ss.str();
}
