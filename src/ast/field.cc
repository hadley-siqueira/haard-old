#include "ast/field.h"

using namespace haard;

Field::Field() {
    initial_value = nullptr;
}

Field::~Field() {
    delete initial_value;
}

Expression *Field::get_initial_value() const {
    return initial_value;
}

void Field::set_initial_value(Expression* value) {
    initial_value = value;
}
