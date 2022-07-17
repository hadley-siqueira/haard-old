#include "ast/identifier.h"

using namespace haard;

Identifier::Identifier() {
    
}

Identifier::Identifier(Token& token) {
    line = token.get_line();
    column = token.get_column();
    lexeme = token.get_lexeme();
}

int Identifier::get_line() {
    return line;
}

int Identifier::get_column() {
    return column;
}

std::string Identifier::get_lexeme() {
    return lexeme;
}

void Identifier::set_line(int line) {
    this->line = line;
}

void Identifier::set_column(int column) {
    this->column;
}

void Identifier::set_lexeme(std::string lexeme) {
    this->lexeme = lexeme;
}
