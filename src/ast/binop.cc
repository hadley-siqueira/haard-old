#include "ast/binop.h"

using namespace haard;

BinOp::BinOp() {
    kind = EXPR_UNKNOWN;
    left = nullptr;
    right = nullptr;
    initial_assign = false;
}

BinOp::BinOp(int kind, Expression* left, Expression* right) {
    this->kind = kind;
    this->left = left;
    this->right = right;
    initial_assign = false;
}

BinOp::BinOp(int kind, Token& token, Expression* left, Expression* right) {
    this->kind = kind;
    this->left = left;
    this->right = right;
    line = token.get_line();
    column = token.get_column();
    initial_assign = false;
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

void BinOp::set_initial_assign(bool value) {
    this->initial_assign = value;
}

bool BinOp::get_initial_assign() {
    return initial_assign;
}
