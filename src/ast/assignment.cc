#include "ast/assignment.h"

using namespace haard;

Assignment::Assignment(Expression* left, Expression* right) {
    this->kind = EXPR_ASSIGN;
    this->left = left;
    this->right = right;
}

Assignment::~Assignment() {
    delete left;
    delete right;
}

Expression* Assignment::get_left() const {
    return left;
}

void Assignment::set_left(Expression* value) {
    left = value;
}

Expression* Assignment::get_right() const {
    return right;
}

void Assignment::set_right(Expression* value) {
    right = value;
}
