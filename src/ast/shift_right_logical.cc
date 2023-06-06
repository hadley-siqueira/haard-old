#include <sstream>
#include "ast/shift_right_logical.h"

using namespace haard;

ShiftRightLogical::ShiftRightLogical(Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_RIGHT_LOGICAL);
    set_left(left);
    set_right(right);
}

ShiftRightLogical::ShiftRightLogical(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_RIGHT_LOGICAL);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
