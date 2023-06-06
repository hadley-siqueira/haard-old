#include <sstream>
#include "ast/bitwise_and.h"

using namespace haard;

BitwiseAnd::BitwiseAnd(Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_AND);
    set_left(left);
    set_right(right);
}

BitwiseAnd::BitwiseAnd(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_BITWISE_AND);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
