#include <sstream>
#include "ast/unary_minus.h"

using namespace haard;

UnaryMinus::UnaryMinus(Expression* expression) {
    set_kind(EXPR_UNARY_MINUS);
    set_expression(expression);
}

UnaryMinus::UnaryMinus(Token& token, Expression* expression) {
    set_kind(EXPR_UNARY_MINUS);
    set_expression(expression);
    set_from_token(token);
}
