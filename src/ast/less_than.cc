#include <sstream>
#include "ast/less_than.h"

using namespace haard;

LessThan::LessThan(Expression* left, Expression* right) {
    this->kind = EXPR_LESS_THAN;
    this->left = left;
    this->right = right;
}

LessThan::LessThan(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_LESS_THAN;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

LessThan::~LessThan() {
    delete left;
    delete right;
}

Expression* LessThan::get_left() const {
    return left;
}

void LessThan::set_left(Expression* value) {
    left = value;
}

Expression* LessThan::get_right() const {
    return right;
}

void LessThan::set_right(Expression* value) {
    right = value;
}

std::string LessThan::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " < ";
    ss << right->to_str();

    return ss.str();
}
