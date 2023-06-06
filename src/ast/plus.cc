#include <sstream>
#include "ast/plus.h"

using namespace haard;

Plus::Plus(Expression* left, Expression* right) {
    set_kind(EXPR_PLUS);
    set_left(left);
    set_right(right);
}

Plus::Plus(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_PLUS);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
