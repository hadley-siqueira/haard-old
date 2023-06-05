#include <sstream>
#include "ast/modulo_assignment.h"

using namespace haard;

ModuloAssignment::ModuloAssignment(Expression* left, Expression* right) {
    this->kind = EXPR_MODULO_ASSIGNMENT;
    this->left = left;
    this->right = right;
}

ModuloAssignment::ModuloAssignment(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_MODULO_ASSIGNMENT;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

ModuloAssignment::~ModuloAssignment() {
    delete left;
    delete right;
}

Expression* ModuloAssignment::get_left() const {
    return left;
}

void ModuloAssignment::set_left(Expression* value) {
    left = value;
}

Expression* ModuloAssignment::get_right() const {
    return right;
}

void ModuloAssignment::set_right(Expression* value) {
    right = value;
}

std::string ModuloAssignment::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " %= ";
    ss << right->to_str();

    return ss.str();
}
