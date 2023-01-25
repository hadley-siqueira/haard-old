#include "ast/literal.h"
#include "utils/utils.h"

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

Literal::Literal(int kind, std::string str) {
    this->kind = kind;
    lexeme = str;
    line = 0;
    column = 0;
}
            
std::string Literal::get_lexeme() {
    return lexeme;
}

Expression* Literal::clone() {
    Literal* other = new Literal(kind);

    other->lexeme = lexeme;
    other->line = line;
    other->column = column;

    return other;
}

uint64_t Literal::to_u64() {
    uint64_t res = 0;
    std::string value(lexeme);

    switch (kind) {
    case EXPR_LITERAL_BOOL:
        if (value == "true") {
            res = 1;
        }
        break;

    case EXPR_LITERAL_CHAR:
        res = char2u64(value);
        break;

    case EXPR_LITERAL_INTEGER:
        res = conv64(value);
        break;

    default:
        break;
    }

    return res;
}

