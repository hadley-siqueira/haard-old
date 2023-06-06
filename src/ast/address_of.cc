#include <sstream>
#include "ast/address_of.h"

using namespace haard;

AddressOf::AddressOf(Expression* expression) {
    set_kind(EXPR_ADDRESS_OF);
    set_expression(expression);
}

AddressOf::AddressOf(Token& token, Expression* expression) {
    set_kind(EXPR_ADDRESS_OF);
    set_expression(expression);
    set_from_token(token);
}
