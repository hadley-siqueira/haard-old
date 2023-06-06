#include "ast/delete.h"

using namespace haard;

Delete::Delete(Token& token, Expression* expression) {
    set_kind(EXPR_DELETE);
    set_from_token(token);
    set_expression(expression);
}
