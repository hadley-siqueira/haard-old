#include <sstream>
#include "ast/plus.h"

using namespace haard;

Plus::Plus(Expression* left, Expression* right) {
    this->kind = EXPR_PLUS;
    this->left = left;
    this->right = right;
}

Plus::Plus(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_PLUS;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

Plus::~Plus() {
    delete left;
    delete right;
}

Expression* Plus::get_left() const {
    return left;
}

void Plus::set_left(Expression* value) {
    left = value;
}

Expression* Plus::get_right() const {
    return right;
}

void Plus::set_right(Expression* value) {
    right = value;
}

std::string Plus::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " + ";
    ss << right->to_str();

    return ss.str();
}
