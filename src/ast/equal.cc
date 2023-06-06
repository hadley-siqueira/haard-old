#include <sstream>
#include "ast/equal.h"

using namespace haard;

Equal::Equal(Expression* left, Expression* right) {
    set_kind(EXPR_EQUAL);
    set_left(left);
    set_right(right);
}

Equal::Equal(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_EQUAL);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
