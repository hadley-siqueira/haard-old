#include "ast/variable.h"

using namespace haard;

Variable::Variable() {
    line = 0;
    column = 0;
    type = nullptr;
}

Variable::Variable(Token& token) {
    name = token.get_lexeme();
    line = token.get_line();
    column = token.get_column();
    type = nullptr;
}

Variable::~Variable() {
    delete type;
}

int Variable::get_line() {
    return line;
}

int Variable::get_column() {
    return column;
}

std::string Variable::get_name() {
    return name;
}

Type* Variable::get_type() {
    return type;
}

void Variable::set_line(int line) {
    this->line = line;
}

void Variable::set_column(int column) {
    this->column = column;
}

void Variable::set_name(std::string name) {
    this->name = name;
}

void Variable::set_type(Type* type) {
    this->type = type;
}

void Variable::set_from_token(Token& token) {
    line = token.get_line();
    column = token.get_column();
    name = token.get_lexeme();
}
