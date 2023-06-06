#include <sstream>
#include "ast/bitwise_or.h"

using namespace haard;

BitwiseOr::BitwiseOr(Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_OR);
    set_left(left);
    set_right(right);
}

BitwiseOr::BitwiseOr(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_OR);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
