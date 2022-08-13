#include "ast/literal.h"

using namespace haard;

Literal::Literal(int kind) {
    this->kind = kind;
    lexeme = nullptr;
}

Literal::Literal(int kind, Token& token) {
    this->kind = kind;
    lexeme = token.get_lexeme();
    line = token.get_line();
    column = token.get_column();
}
            
const char* Literal::get_lexeme() {
    return lexeme;
}

Expression* Literal::clone() {
    Literal* other = new Literal(kind);

    other->lexeme = lexeme;
    other->line = line;
    other->column = column;

    return other;
}
