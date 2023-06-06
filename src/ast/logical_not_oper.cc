#include <sstream>
#include "ast/logical_not_oper.h"

using namespace haard;

LogicalNotOper::LogicalNotOper(Expression* expression) {
    set_kind(EXPR_LOGICAL_NOT_OPER);
    set_expression(expression);
}

LogicalNotOper::LogicalNotOper(Token& token, Expression* expression) {
    set_kind(EXPR_LOGICAL_NOT_OPER);
    set_expression(expression);
    set_from_token(token);
}
