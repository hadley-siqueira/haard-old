#include <sstream>
#include "ast/greater_than.h"

using namespace haard;

GreaterThan::GreaterThan(Expression* left, Expression* right) {
    this->kind = EXPR_GREATER_THAN;
    this->left = left;
    this->right = right;
}

GreaterThan::GreaterThan(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_GREATER_THAN;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

GreaterThan::~GreaterThan() {
    delete left;
    delete right;
}

Expression* GreaterThan::get_left() const {
    return left;
}

void GreaterThan::set_left(Expression* value) {
    left = value;
}

Expression* GreaterThan::get_right() const {
    return right;
}

void GreaterThan::set_right(Expression* value) {
    right = value;
}

std::string GreaterThan::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " > ";
    ss << right->to_str();

    return ss.str();
}
