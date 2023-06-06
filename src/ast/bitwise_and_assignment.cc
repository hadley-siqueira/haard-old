#include <sstream>
#include "ast/bitwise_and_assignment.h"

using namespace haard;

BitwiseAndAssignment::BitwiseAndAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_AND_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

BitwiseAndAssignment::BitwiseAndAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_AND_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
