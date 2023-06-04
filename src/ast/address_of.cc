#include <sstream>
#include "ast/address_of.h"

using namespace haard;

AddressOf::AddressOf(Expression* expression) {
    this->kind = EXPR_ADDRESS_OF;
    this->expression = expression;
}

AddressOf::AddressOf(Token& token, Expression* expression) {
    this->kind = EXPR_ADDRESS_OF;
    this->expression = expression;
    this->line = token.get_line();
    this->column = token.get_column();
}

AddressOf::~AddressOf() {
    delete expression;
}

Expression* AddressOf::get_expression() const {
    return expression;
}

void AddressOf::set_expression(Expression* value) {
    expression = value;
}

std::string AddressOf::to_str() {
    std::stringstream ss;

    ss << "&" << expression->to_str();

    return ss.str();
}
