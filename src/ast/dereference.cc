#include <sstream>
#include "ast/dereference.h"

using namespace haard;

Dereference::Dereference(Expression* expression) {
    set_kind(EXPR_DEREFERENCE);
    set_expression(expression);
}

Dereference::Dereference(Token& token, Expression* expression) {
    set_kind(EXPR_DEREFERENCE);
    set_expression(expression);
    set_from_token(token);
}
