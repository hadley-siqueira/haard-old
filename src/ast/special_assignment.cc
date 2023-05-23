#include <sstream>
#include "ast/special_assignment.h"

using namespace haard;

SpecialAssignment::SpecialAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_SPECIAL_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

SpecialAssignment::SpecialAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_SPECIAL_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

SpecialAssignment::~SpecialAssignment() {
    delete left;
    delete right;
}

Expression* SpecialAssignment::get_left() const {
    return left;
}

void SpecialAssignment::set_left(Expression* value) {
    left = value;
}

Expression* SpecialAssignment::get_right() const {
    return right;
}

void SpecialAssignment::set_right(Expression* value) {
    right = value;
}

std::string SpecialAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " := ";
    ss << right->to_str();

    return ss.str();
}
