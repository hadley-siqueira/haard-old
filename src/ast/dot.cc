#include <sstream>
#include "ast/dot.h"

using namespace haard;

Dot::Dot(Expression* object, Expression* field) {
    set_kind(EXPR_DOT);
    set_object(object);
    set_field(field);
}

Dot::Dot(Token& token, Expression* object, Expression* field) {
    set_kind(EXPR_DOT);
    set_object(object);
    set_field(field);
    set_from_token(token);
}

Expression* Dot::get_object() const {
    return get_left();
}

void Dot::set_object(Expression* value) {
    set_left(value);
}

Expression* Dot::get_field() const {
    return get_right();
}

void Dot::set_field(Expression* value) {
    set_right(value);
}
