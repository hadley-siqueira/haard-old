#include "ast/binop.h"

using namespace haard;

BinOp::BinOp() {
    set_kind(EXPR_UNKNOWN);
    left = nullptr;
    right = nullptr;
    initial_assign = false;
}

BinOp::BinOp(int kind, Expression* left, Expression* right) {
    set_kind(kind);
    this->left = left;
    this->right = right;
    initial_assign = false;
}

BinOp::BinOp(int kind, Token& token, Expression* left, Expression* right) {
    set_kind(kind);
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

Symbol* BinOp::get_constructor_call() const {
    return constructor_call;
}

void BinOp::set_constructor_call(Symbol* value) {
    constructor_call = value;
}

int BinOp::get_overloaded_index() const {
    return overloaded_index;
}

void BinOp::set_overloaded_index(int value) {
    overloaded_index = value;
}

