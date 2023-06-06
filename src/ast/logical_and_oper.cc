#include <sstream>
#include "ast/logical_and_oper.h"

using namespace haard;

LogicalAndOper::LogicalAndOper(Expression* left, Expression* right) {
    set_kind(EXPR_LOGICAL_AND_OPER);
    set_left(left);
    set_right(right);
}

LogicalAndOper::LogicalAndOper(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_LOGICAL_AND_OPER);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
