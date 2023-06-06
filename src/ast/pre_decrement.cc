#include <sstream>
#include "ast/pre_decrement.h"

using namespace haard;

PreDecrement::PreDecrement(Expression* expression) {
    set_kind(EXPR_PRE_DECREMENT);
    set_expression(expression);
}

PreDecrement::PreDecrement(Token& token, Expression* expression) {
    set_kind(EXPR_PRE_DECREMENT);
    set_expression(expression);
    set_from_token(token);
}
