#include <sstream>
#include "ast/bitwise_or_assignment.h"

using namespace haard;

BitwiseOrAssignment::BitwiseOrAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_OR_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

BitwiseOrAssignment::BitwiseOrAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_OR_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
