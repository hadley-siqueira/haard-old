#include <sstream>
#include "ast/logical_not.h"

using namespace haard;

LogicalNot::LogicalNot(Expression* expression) {
    set_kind(EXPR_LOGICAL_NOT);
    set_expression(expression);
}

LogicalNot::LogicalNot(Token& token, Expression* expression) {
    set_kind(EXPR_LOGICAL_NOT);
    set_expression(expression);
    set_from_token(token);
}
