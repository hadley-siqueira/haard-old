#include <sstream>
#include "ast/bitwise_xor_assignment.h"

using namespace haard;

BitwiseXorAssignment::BitwiseXorAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_XOR_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

BitwiseXorAssignment::BitwiseXorAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_XOR_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
