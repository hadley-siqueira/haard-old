#include <sstream>
#include "ast/greater_than.h"

using namespace haard;

GreaterThan::GreaterThan(Expression* left, Expression* right) {
    set_kind(EXPR_GREATER_THAN);
    set_left(left);
    set_right(right);
}

GreaterThan::GreaterThan(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_GREATER_THAN);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
