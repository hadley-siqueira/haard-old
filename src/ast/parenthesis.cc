#include <sstream>
#include "ast/parenthesis.h"

using namespace haard;

Parenthesis::Parenthesis(Expression* expression) {
    set_kind(EXPR_PARENTHESIS);
    set_expression(expression);
}

Parenthesis::Parenthesis(Token& token, Expression* expression) {
    set_kind(EXPR_PARENTHESIS);
    set_expression(expression);
    set_from_token(token);
}
