#include "ast/delete_array.h"

using namespace haard;

DeleteArray::DeleteArray(Token& token, Expression* expression) {
    set_kind(EXPR_DELETE_ARRAY);
    set_from_token(token);
    set_expression(expression);
}
