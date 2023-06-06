#include <sstream>
#include "ast/integer_division.h"

using namespace haard;

IntegerDivision::IntegerDivision(Expression* left, Expression* right) {
    set_kind(EXPR_INTEGER_DIVISION);
    set_left(left);
    set_right(right);
}

IntegerDivision::IntegerDivision(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_INTEGER_DIVISION);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
