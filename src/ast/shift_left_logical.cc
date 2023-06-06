#include <sstream>
#include "ast/shift_left_logical.h"

using namespace haard;

ShiftLeftLogical::ShiftLeftLogical(Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_LEFT_LOGICAL);
    set_left(left);
    set_right(right);
}

ShiftLeftLogical::ShiftLeftLogical(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_LEFT_LOGICAL);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
