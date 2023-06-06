#include <sstream>
#include "ast/sizeof.h"

using namespace haard;

Sizeof::Sizeof(Expression* expression) {
    set_kind(EXPR_SIZEOF);
    set_expression(expression);
}

Sizeof::Sizeof(Token& token, Expression* expression) {
    set_kind(EXPR_SIZEOF);
    set_expression(expression);
    set_from_token(token);
}
