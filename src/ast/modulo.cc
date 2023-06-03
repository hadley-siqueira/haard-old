#include <sstream>
#include "ast/modulo.h"

using namespace haard;

Modulo::Modulo(Expression* left, Expression* right) {
    this->kind = EXPR_MODULO;
    this->left = left;
    this->right = right;
}

Modulo::Modulo(Token& token, Expression* left, Expression* right) {
    this->kind = EXPR_MODULO;
    this->left = left;
    this->right = right;
    this->line = token.get_line();
    this->column = token.get_column();
}

Modulo::~Modulo() {
    delete left;
    delete right;
}

Expression* Modulo::get_left() const {
    return left;
}

void Modulo::set_left(Expression* value) {
    left = value;
}

Expression* Modulo::get_right() const {
    return right;
}

void Modulo::set_right(Expression* value) {
    right = value;
}

std::string Modulo::to_str() {
    std::stringstream ss;

    ss << left->to_str();
    ss << " % ";
    ss << right->to_str();

    return ss.str();
}
