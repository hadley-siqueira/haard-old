#include <sstream>
#include "ast/bitwise_not_assignment.h"

using namespace haard;

BitwiseNotAssignment::BitwiseNotAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_NOT_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

BitwiseNotAssignment::BitwiseNotAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_NOT_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
