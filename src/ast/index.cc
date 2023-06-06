#include <sstream>
#include "ast/index.h"

using namespace haard;

Index::Index(Expression* object, Expression* index) {
    set_kind(EXPR_INDEX);
    set_object(object);
    set_index(index);
}

Index::Index(Token& token, Expression* object, Expression* index) {
    set_kind(EXPR_INDEX);
    set_object(object);
    set_index(index);
    set_from_token(token);
}

Expression* Index::get_object() const {
    return get_left();
}

void Index::set_object(Expression* value) {
    set_left(value);
}

Expression* Index::get_index() const {
    return get_right();
}

void Index::set_index(Expression* value) {
    set_right(value);
}
