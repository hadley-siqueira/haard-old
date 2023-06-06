#include <sstream>
#include "ast/modulo.h"

using namespace haard;

Modulo::Modulo(Expression* left, Expression* right) {
    set_kind(EXPR_MODULO);
    set_left(left);
    set_right(right);
}

Modulo::Modulo(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_MODULO);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
