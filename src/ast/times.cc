#include <sstream>
#include "ast/times.h"

using namespace haard;

Times::Times(Expression* left, Expression* right) {
    this->kind = EXPR_TIMES;
    this->left = left;
    this->right = right;
}

Times::Times(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_TIMES;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

Times::~Times() {
    delete left;
    delete right;
}

Expression* Times::get_left() const {
    return left;
}

void Times::set_left(Expression* value) {
    left = value;
}

Expression* Times::get_right() const {
    return right;
}

void Times::set_right(Expression* value) {
    right = value;
}

std::string Times::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " * ";
    ss << right->to_str();

    return ss.str();
}
