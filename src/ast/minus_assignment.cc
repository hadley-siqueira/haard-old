#include <sstream>
#include "ast/minus_assignment.h"

using namespace haard;

MinusAssignment::MinusAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_MINUS_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

MinusAssignment::MinusAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_MINUS_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

MinusAssignment::~MinusAssignment() {
    delete left;
    delete right;
}

Expression* MinusAssignment::get_left() const {
    return left;
}

void MinusAssignment::set_left(Expression* value) {
    left = value;
}

Expression* MinusAssignment::get_right() const {
    return right;
}

void MinusAssignment::set_right(Expression* value) {
    right = value;
}

std::string MinusAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " -= ";
    ss << right->to_str();

    return ss.str();
}
