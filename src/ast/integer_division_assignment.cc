#include <sstream>
#include "ast/integer_division_assignment.h"

using namespace haard;

IntegerDivisionAssignment::IntegerDivisionAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_INTEGER_DIVISION_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

IntegerDivisionAssignment::IntegerDivisionAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_INTEGER_DIVISION_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

IntegerDivisionAssignment::~IntegerDivisionAssignment() {
    delete left;
    delete right;
}

Expression* IntegerDivisionAssignment::get_left() const {
    return left;
}

void IntegerDivisionAssignment::set_left(Expression* value) {
    left = value;
}

Expression* IntegerDivisionAssignment::get_right() const {
    return right;
}

void IntegerDivisionAssignment::set_right(Expression* value) {
    right = value;
}

std::string IntegerDivisionAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " //= ";
    ss << right->to_str();

    return ss.str();
}
