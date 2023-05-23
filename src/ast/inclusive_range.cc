#include <sstream>
#include "ast/inclusive_range.h"

using namespace haard;

InclusiveRange::InclusiveRange(Expression* left, Expression* right) {
    this->kind = EXPR_INCLUSIVE_RANGE;
    this->left = left;
    this->right = right;
}

InclusiveRange::InclusiveRange(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_INCLUSIVE_RANGE;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

InclusiveRange::~InclusiveRange() {
    delete left;
    delete right;
}

Expression* InclusiveRange::get_left() const {
    return left;
}

void InclusiveRange::set_left(Expression* value) {
    left = value;
}

Expression* InclusiveRange::get_right() const {
    return right;
}

void InclusiveRange::set_right(Expression* value) {
    right = value;
}

std::string InclusiveRange::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " .. ";
    ss << right->to_str();

    return ss.str();
}
