#include <sstream>
#include "ast/assignment.h"

using namespace haard;

Assignment::Assignment(Expression* left, Expression* right) {
    set_kind(EXPR_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_initial_value(false);
}

Assignment::Assignment(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_ASSIGNMENT);
    set_left(left);
    set_right(right);
    set_from_token(token);
    set_initial_value(false);
}

bool Assignment::get_initial_value() const {
    return initial_value;
}

void Assignment::set_initial_value(bool value) {
    initial_value = value;
}
