#include <sstream>
#include "ast/bitwise_not.h"

using namespace haard;

BitwiseNot::BitwiseNot(Expression* expression) {
    set_kind(EXPR_BITWISE_NOT);
    set_expression(expression);
}

BitwiseNot::BitwiseNot(Token& token, Expression* expression) {
    set_kind(EXPR_BITWISE_NOT);
    set_expression(expression);
    set_from_token(token);
}
