#include <sstream>
#include "ast/power.h"

using namespace haard;

Power::Power(Expression* left, Expression* right) {
    this->kind = EXPR_POWER;
    this->left = left;
    this->right = right;
}

Power::Power(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_POWER;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

Power::~Power() {
    delete left;
    delete right;
}

Expression* Power::get_left() const {
    return left;
}

void Power::set_left(Expression* value) {
    left = value;
}

Expression* Power::get_right() const {
    return right;
}

void Power::set_right(Expression* value) {
    right = value;
}

std::string Power::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " ** ";
    ss << right->to_str();

    return ss.str();
}
