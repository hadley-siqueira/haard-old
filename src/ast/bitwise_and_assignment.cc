#include <sstream>
#include "ast/bitwise_and_assignment.h"

using namespace haard;

BitwiseAndAssignment::BitwiseAndAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_AND_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

BitwiseAndAssignment::BitwiseAndAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_BITWISE_AND_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

BitwiseAndAssignment::~BitwiseAndAssignment() {
    delete left;
    delete right;
}

Expression* BitwiseAndAssignment::get_left() const {
    return left;
}

void BitwiseAndAssignment::set_left(Expression* value) {
    left = value;
}

Expression* BitwiseAndAssignment::get_right() const {
    return right;
}

void BitwiseAndAssignment::set_right(Expression* value) {
    right = value;
}

std::string BitwiseAndAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " &= ";
    ss << right->to_str();

    return ss.str();
}
