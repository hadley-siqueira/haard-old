#include <sstream>
#include "ast/pre_increment.h"

using namespace haard;

PreIncrement::PreIncrement(Expression* expression) {
    set_kind(EXPR_PRE_INCREMENT);
    set_expression(expression);
}

PreIncrement::PreIncrement(Token& token, Expression* expression) {
    set_kind(EXPR_PRE_INCREMENT);
    set_expression(expression);
    set_from_token(token);
}
