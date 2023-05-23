#include <sstream>
#include "ast/integer_division.h"

using namespace haard;

IntegerDivision::IntegerDivision(Expression* left, Expression* right) {
    this->kind = EXPR_INTEGER_DIVISION;
    this->left = left;
    this->right = right;
}

IntegerDivision::IntegerDivision(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_INTEGER_DIVISION;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

IntegerDivision::~IntegerDivision() {
    delete left;
    delete right;
}

Expression* IntegerDivision::get_left() const {
    return left;
}

void IntegerDivision::set_left(Expression* value) {
    left = value;
}

Expression* IntegerDivision::get_right() const {
    return right;
}

void IntegerDivision::set_right(Expression* value) {
    right = value;
}

std::string IntegerDivision::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " * ";
    ss << right->to_str();

    return ss.str();
}
