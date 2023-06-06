#include <sstream>
#include "ast/shift_right_logical_assignment.h"

using namespace haard;

ShiftRightLogicalAssignment::ShiftRightLogicalAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_RIGHT_LOGICAL_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

ShiftRightLogicalAssignment::ShiftRightLogicalAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_SHIFT_RIGHT_LOGICAL_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
