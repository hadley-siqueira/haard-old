#include "token/token.h"

using namespace haard;

int Token::get_kind() {
    return kind;
}

int Token::get_line() {
    return line;
}

int Token::get_column() {
    return column;
}

int Token::get_whitespace() {
    return whitespace;
}

std::string Token::get_lexeme() {
    return lexeme;
}

void Token::set_kind(int kind) {
    this->kind = kind;
}

void Token::set_line(int line) {
    this->line = line;
}

void Token::set_column(int column) {
    this->column = column;
}

void Token::set_whitespace(int whitespace) {
    this->whitespace = whitespace;
}

void Token::set_lexeme(std::string lexeme) {
    this->lexeme = lexeme;
}
