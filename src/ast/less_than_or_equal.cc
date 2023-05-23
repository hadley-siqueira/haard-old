#include <sstream>
#include "ast/less_than_or_equal.h"

using namespace haard;

LessThanOrEqual::LessThanOrEqual(Expression* left, Expression* right) {
    this->kind = EXPR_LESS_THAN_OR_EQUAL;
    this->left = left;
    this->right = right;
}

LessThanOrEqual::LessThanOrEqual(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_LESS_THAN_OR_EQUAL;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

LessThanOrEqual::~LessThanOrEqual() {
    delete left;
    delete right;
}

Expression* LessThanOrEqual::get_left() const {
    return left;
}

void LessThanOrEqual::set_left(Expression* value) {
    left = value;
}

Expression* LessThanOrEqual::get_right() const {
    return right;
}

void LessThanOrEqual::set_right(Expression* value) {
    right = value;
}

std::string LessThanOrEqual::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " <= ";
    ss << right->to_str();

    return ss.str();
}
