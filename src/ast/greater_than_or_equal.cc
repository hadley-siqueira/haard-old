#include <sstream>
#include "ast/greater_than_or_equal.h"

using namespace haard;

GreaterThanOrEqual::GreaterThanOrEqual(Expression* left, Expression* right) {
    set_kind(EXPR_GREATER_THAN_OR_EQUAL);
    set_left(left);
    set_right(right);
}

GreaterThanOrEqual::GreaterThanOrEqual(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_GREATER_THAN_OR_EQUAL);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
