#include "ast/field.h"

using namespace haard;

Field::Field() {
    type = nullptr;
    initial_value = nullptr;
}

Field::~Field() {
    delete initial_value;
}

Type* Field::get_type() const {
    return type;
}

Expression *Field::get_initial_value() const {
    return initial_value;
}

void Field::set_type(Type* value) {
    type = value;
}

void Field::set_initial_value(Expression* value) {
    initial_value = value;
}

void Field::set_from_token(Token& token) {
    line = token.get_line();
    column = token.get_column();
    name = token.get_lexeme();
}
