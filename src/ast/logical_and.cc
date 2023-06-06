#include <sstream>
#include "ast/logical_and.h"

using namespace haard;

LogicalAnd::LogicalAnd(Expression* left, Expression* right) {
    set_kind(EXPR_LOGICAL_AND);
    set_left(left);
    set_right(right);
}

LogicalAnd::LogicalAnd(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_LOGICAL_AND);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
