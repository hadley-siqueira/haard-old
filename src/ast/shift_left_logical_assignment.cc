#include <sstream>
#include "ast/shift_left_logical_assignment.h"

using namespace haard;

ShiftLeftLogicalAssignment::ShiftLeftLogicalAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_LEFT_LOGICAL_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

ShiftLeftLogicalAssignment::ShiftLeftLogicalAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_LEFT_LOGICAL_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
