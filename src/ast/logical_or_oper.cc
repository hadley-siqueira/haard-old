#include <sstream>
#include "ast/logical_or_oper.h"

using namespace haard;

LogicalOrOper::LogicalOrOper(Expression* left, Expression* right) {
    set_kind(EXPR_LOGICAL_OR_OPER);
    set_left(left);
    set_right(right);
}

LogicalOrOper::LogicalOrOper(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_LOGICAL_OR_OPER);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
