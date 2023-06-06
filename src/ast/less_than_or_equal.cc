#include <sstream>
#include "ast/less_than_or_equal.h"

using namespace haard;

LessThanOrEqual::LessThanOrEqual(Expression* left, Expression* right) {
    set_kind(EXPR_LESS_THAN_OR_EQUAL);
    set_left(left);
    set_right(right);
}

LessThanOrEqual::LessThanOrEqual(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_LESS_THAN_OR_EQUAL);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
