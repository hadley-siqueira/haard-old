#include <sstream>
#include "ast/pos_increment.h"

using namespace haard;

PosIncrement::PosIncrement(Expression* expression) {
    set_kind(EXPR_POS_INCREMENT);
    set_expression(expression);
}

PosIncrement::PosIncrement(Token& token, Expression* expression) {
    set_kind(EXPR_POS_INCREMENT);
    set_expression(expression);
    set_from_token(token);
}
