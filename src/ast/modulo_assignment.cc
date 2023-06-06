#include <sstream>
#include "ast/modulo_assignment.h"

using namespace haard;

ModuloAssignment::ModuloAssignment(Expression* left, Expression* right) {
    set_kind(EXPR_MODULO_ASSIGNMENT);
    set_left(left);
    set_right(right);
}

ModuloAssignment::ModuloAssignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_MODULO_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
