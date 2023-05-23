#include <sstream>
#include "ast/equal.h"

using namespace haard;

Equal::Equal(Expression* left, Expression* right) {
    this->kind = EXPR_EQUAL;
    this->left = left;
    this->right = right;
}

Equal::Equal(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_EQUAL;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

Equal::~Equal() {
    delete left;
    delete right;
}

Expression* Equal::get_left() const {
    return left;
}

void Equal::set_left(Expression* value) {
    left = value;
}

Expression* Equal::get_right() const {
    return right;
}

void Equal::set_right(Expression* value) {
    right = value;
}

std::string Equal::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " == ";
    ss << right->to_str();

    return ss.str();
}
