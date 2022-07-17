
#include "ast/binop.h"

using namespace haard;

BinOp::BinOp() {
    kind = EXPR_UNKNOWN;
    left = nullptr;
    right = nullptr;
}

BinOp::BinOp(int kind, Expression* left, Expression* right) {
    this->kind = kind;
    this->left = left;
    this->right = right;
}

BinOp::~BinOp() {
    delete left;
    delete right;
}

Expression* BinOp::get_left() {
    return left;
}

Expression* BinOp::get_right() {
    return right;
}

void BinOp::set_left(Expression* left) {
    this->left = left;
}

void BinOp::set_right(Expression* right) {
    this->right = right;
}

