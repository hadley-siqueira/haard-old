#include <sstream>
#include "ast/arrow.h"

using namespace haard;

Arrow::Arrow(Expression* object, Expression* field) {
    set_kind(EXPR_ARROW);
    set_object(object);
    set_field(field);
}

Arrow::Arrow(Token& token, Expression* object, Expression* field) {
    set_kind(EXPR_ARROW);
    set_object(object);
    set_field(field);
    set_from_token(token);
}

Expression* Arrow::get_object() const {
    return get_left();
}

void Arrow::set_object(Expression* value) {
    set_left(value);
}

Expression* Arrow::get_field() const {
    return get_right();
}

void Arrow::set_field(Expression* value) {
    set_right(value);
}
