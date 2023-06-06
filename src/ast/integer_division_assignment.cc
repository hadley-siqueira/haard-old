#include <sstream>
#include "ast/integer_division_assignment.h"

using namespace haard;

IntegerDivisionAssignment::IntegerDivisionAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_INTEGER_DIVISION_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

IntegerDivisionAssignment::IntegerDivisionAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_INTEGER_DIVISION_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
