#include "ast/literal.h"

using namespace haard;

Literal::Literal(int kind, Token& token) {
    this->kind = kind;
    lexeme = token.get_lexeme();
    line = token.get_line();
    column = token.get_column();
}
            
const char* Literal::get_lexeme() {
    return lexeme;
}
