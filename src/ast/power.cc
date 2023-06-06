#include <sstream>
#include "ast/power.h"

using namespace haard;

Power::Power(Expression* left, Expression* right) {
    set_kind(EXPR_POWER);
    set_left(left);
    set_right(right);
}

Power::Power(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_POWER);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
