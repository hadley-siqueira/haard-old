#include <sstream>
#include "ast/division_assignment.h"

using namespace haard;

DivisionAssignment::DivisionAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_DIVISION_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

DivisionAssignment::DivisionAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_DIVISION_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

DivisionAssignment::~DivisionAssignment() {
    delete left;
    delete right;
}

Expression* DivisionAssignment::get_left() const {
    return left;
}

void DivisionAssignment::set_left(Expression* value) {
    left = value;
}

Expression* DivisionAssignment::get_right() const {
    return right;
}

void DivisionAssignment::set_right(Expression* value) {
    right = value;
}

std::string DivisionAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " /= ";
    ss << right->to_str();

    return ss.str();
}
