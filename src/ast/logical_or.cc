#include <sstream>
#include "ast/logical_or.h"

using namespace haard;

LogicalOr::LogicalOr(Expression* left, Expression* right) {
    set_kind(EXPR_LOGICAL_OR);
    set_left(left);
    set_right(right);
}

LogicalOr::LogicalOr(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_LOGICAL_OR);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
