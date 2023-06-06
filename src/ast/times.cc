#include <sstream>
#include "ast/times.h"

using namespace haard;

Times::Times(Expression* left, Expression* right) {
    set_kind(EXPR_TIMES);
    set_left(left);
    set_right(right);
}

Times::Times(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_TIMES);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
