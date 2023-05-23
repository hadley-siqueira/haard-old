#include <sstream>
#include "ast/exclusive_range.h"

using namespace haard;

ExclusiveRange::ExclusiveRange(Expression* left, Expression* right) {
    this->kind = EXPR_EXCLUSIVE_RANGE;
    this->left = left;
    this->right = right;
}

ExclusiveRange::ExclusiveRange(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_EXCLUSIVE_RANGE;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

ExclusiveRange::~ExclusiveRange() {
    delete left;
    delete right;
}

Expression* ExclusiveRange::get_left() const {
    return left;
}

void ExclusiveRange::set_left(Expression* value) {
    left = value;
}

Expression* ExclusiveRange::get_right() const {
    return right;
}

void ExclusiveRange::set_right(Expression* value) {
    right = value;
}

std::string ExclusiveRange::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " ... ";
    ss << right->to_str();

    return ss.str();
}
