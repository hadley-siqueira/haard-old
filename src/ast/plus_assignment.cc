#include <sstream>
#include "ast/plus_assignment.h"

using namespace haard;

PlusAssignment::PlusAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_PLUS_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

PlusAssignment::PlusAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_PLUS_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

PlusAssignment::~PlusAssignment() {
    delete left;
    delete right;
}

Expression* PlusAssignment::get_left() const {
    return left;
}

void PlusAssignment::set_left(Expression* value) {
    left = value;
}

Expression* PlusAssignment::get_right() const {
    return right;
}

void PlusAssignment::set_right(Expression* value) {
    right = value;
}

std::string PlusAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " += ";
    ss << right->to_str();

    return ss.str();
}
