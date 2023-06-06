#include "ast/literal.h"
#include "utils/utils.h"

using namespace haard;

Literal::Literal(int kind) {
    set_kind(kind);
}

Literal::Literal(int kind, Token& token) {
    set_kind(kind);
    lexeme = token.get_lexeme();
    line = token.get_line();
    column = token.get_column();
}

Literal::Literal(int kind, std::string str) {
    set_kind(kind);
    lexeme = str;
    line = 0;
    column = 0;
}
            
std::string Literal::get_lexeme() {
    return lexeme;
}

uint64_t Literal::to_u64() {
    uint64_t res = 0;
    std::string value(lexeme);

    switch (get_kind()) {
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
