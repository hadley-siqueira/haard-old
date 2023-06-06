#include <sstream>
#include "ast/unary_plus.h"

using namespace haard;

UnaryPlus::UnaryPlus(Expression* expression) {
    set_kind(EXPR_UNARY_PLUS);
    set_expression(expression);
}

UnaryPlus::UnaryPlus(Token& token, Expression* expression) {
    set_kind(EXPR_UNARY_PLUS);
    set_expression(expression);
    set_from_token(token);
}
