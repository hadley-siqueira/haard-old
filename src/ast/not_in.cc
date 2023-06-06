#include <sstream>
#include "ast/not_in.h"

using namespace haard;

NotIn::NotIn(Expression* left, Expression* right) {
    set_kind(EXPR_NOT_IN);
    set_left(left);
    set_right(right);
}

NotIn::NotIn(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_NOT_IN);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
