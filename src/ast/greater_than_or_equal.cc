#include <sstream>
#include "ast/greater_than_or_equal.h"

using namespace haard;

GreaterThanOrEqual::GreaterThanOrEqual(Expression* left, Expression* right) {
    this->kind = EXPR_GREATER_THAN_OR_EQUAL;
    this->left = left;
    this->right = right;
}

GreaterThanOrEqual::GreaterThanOrEqual(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_GREATER_THAN_OR_EQUAL;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

GreaterThanOrEqual::~GreaterThanOrEqual() {
    delete left;
    delete right;
}

Expression* GreaterThanOrEqual::get_left() const {
    return left;
}

void GreaterThanOrEqual::set_left(Expression* value) {
    left = value;
}

Expression* GreaterThanOrEqual::get_right() const {
    return right;
}

void GreaterThanOrEqual::set_right(Expression* value) {
    right = value;
}

std::string GreaterThanOrEqual::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " >= ";
    ss << right->to_str();

    return ss.str();
}
