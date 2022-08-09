#include "ast/identifier.h"

using namespace haard;

Identifier::Identifier() {
    kind = EXPR_ID;
    symbol = nullptr;
    lexeme = nullptr;
}

Identifier::Identifier(Token& token) {
    kind = EXPR_ID;
    line = token.get_line();
    column = token.get_column();
    lexeme = token.get_lexeme();
    symbol = nullptr;
}

int Identifier::get_line() {
    return line;
}

int Identifier::get_column() {
    return column;
}

const char* Identifier::get_lexeme() {
    return lexeme;
}

Symbol* Identifier::get_symbol() {
    return symbol;
}

void Identifier::set_line(int line) {
    this->line = line;
}

void Identifier::set_column(int column) {
    this->column;
}

void Identifier::set_lexeme(const char* lexeme) {
    this->lexeme = lexeme;
}

void Identifier::set_symbol(Symbol* symbol) {
    this->symbol = symbol;
}
