#include <sstream>
#include "ast/exclusive_range.h"

using namespace haard;

ExclusiveRange::ExclusiveRange(Expression* left, Expression* right) {
    set_kind(EXPR_EXCLUSIVE_RANGE);
    set_left(left);
    set_right(right);
}

ExclusiveRange::ExclusiveRange(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_EXCLUSIVE_RANGE);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
