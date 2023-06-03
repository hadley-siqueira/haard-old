#include <sstream>
#include "ast/division.h"

using namespace haard;

Division::Division(Expression* left, Expression* right) {
    this->kind = EXPR_DIVISION;
    this->left = left;
    this->right = right;
}

Division::Division(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_DIVISION;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

Division::~Division() {
    delete left;
    delete right;
}

Expression* Division::get_left() const {
    return left;
}

void Division::set_left(Expression* value) {
    left = value;
}

Expression* Division::get_right() const {
    return right;
}

void Division::set_right(Expression* value) {
    right = value;
}

std::string Division::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " / ";
    ss << right->to_str();

    return ss.str();
}
