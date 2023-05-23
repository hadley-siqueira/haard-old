#include <sstream>
#include "ast/times_assignment.h"

using namespace haard;

TimesAssignment::TimesAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_TIMES_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

TimesAssignment::TimesAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_TIMES_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

TimesAssignment::~TimesAssignment() {
    delete left;
    delete right;
}

Expression* TimesAssignment::get_left() const {
    return left;
}

void TimesAssignment::set_left(Expression* value) {
    left = value;
}

Expression* TimesAssignment::get_right() const {
    return right;
}

void TimesAssignment::set_right(Expression* value) {
    right = value;
}

std::string TimesAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " *= ";
    ss << right->to_str();

    return ss.str();
}
