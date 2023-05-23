#include <sstream>
#include "ast/not_equal.h"

using namespace haard;

NotEqual::NotEqual(Expression* left, Expression* right) {
    this->kind = EXPR_NOT_EQUAL;
    this->left = left;
    this->right = right;
}

NotEqual::NotEqual(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_NOT_EQUAL;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

NotEqual::~NotEqual() {
    delete left;
    delete right;
}

Expression* NotEqual::get_left() const {
    return left;
}

void NotEqual::set_left(Expression* value) {
    left = value;
}

Expression* NotEqual::get_right() const {
    return right;
}

void NotEqual::set_right(Expression* value) {
    right = value;
}

std::string NotEqual::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " != ";
    ss << right->to_str();

    return ss.str();
}
