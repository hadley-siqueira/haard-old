#include <sstream>
#include "ast/inclusive_range.h"

using namespace haard;

InclusiveRange::InclusiveRange(Expression* left, Expression* right) {
    set_kind(EXPR_INCLUSIVE_RANGE);
    set_left(left);
    set_right(right);
}

InclusiveRange::InclusiveRange(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_INCLUSIVE_RANGE);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
