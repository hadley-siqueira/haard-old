#include <sstream>
#include "ast/pos_decrement.h"

using namespace haard;

PosDecrement::PosDecrement(Expression* expression) {
    set_kind(EXPR_POS_DECREMENT);
    set_expression(expression);
}

PosDecrement::PosDecrement(Token& token, Expression* expression) {
    set_kind(EXPR_POS_DECREMENT);
    set_expression(expression);
    set_from_token(token);
}
