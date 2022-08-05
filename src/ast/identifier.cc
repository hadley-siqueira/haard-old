#include "ast/identifier.h"

using namespace haard;

Identifier::Identifier() {
    kind = EXPR_ID;
}

Identifier::Identifier(Token& token) {
    kind = EXPR_ID;
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

const char* Identifier::get_lexeme() {
    return lexeme;
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
