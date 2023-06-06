#include <sstream>
#include "ast/shift_right_arithmetic.h"

using namespace haard;

ShiftRightArithmetic::ShiftRightArithmetic(Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_RIGHT_ARITHMETIC);
    set_left(left);
    set_right(right);
}

ShiftRightArithmetic::ShiftRightArithmetic(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_RIGHT_ARITHMETIC);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
