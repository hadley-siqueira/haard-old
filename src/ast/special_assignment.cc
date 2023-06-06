#include <sstream>
#include "ast/special_assignment.h"

using namespace haard;

SpecialAssignment::SpecialAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_SPECIAL_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

SpecialAssignment::SpecialAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_SPECIAL_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
