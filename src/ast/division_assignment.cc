#include <sstream>
#include "ast/division_assignment.h"

using namespace haard;

DivisionAssignment::DivisionAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_DIVISION_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

DivisionAssignment::DivisionAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_DIVISION_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
