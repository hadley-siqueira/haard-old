#include "ast/type.h"

using namespace haard;

Type::Type() {

}

Type::Type(int kind) {
    this->kind = kind;
}

Type::Type(int kind, Token& token) {
    this->kind = kind;
    line = token.get_line();
    column = token.get_column();
}

Type::~Type() {

}

int Type::get_kind() {
    return kind;
}

int Type::get_line() {
    return line;
}

int Type::get_column() {
    return column;
}

void Type::set_kind(int kind) {
    this->kind = kind;
}

void Type::set_line(int line) {
    this->line = line;
}

void Type::set_column(int column) {
    this->column = column;
}
