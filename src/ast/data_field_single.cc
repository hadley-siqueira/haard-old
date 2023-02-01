#include "ast/data_field_single.h"

using namespace haard;

Type* DataFieldSingle::get_type() const {
    return type;
}

void DataFieldSingle::set_type(Type* value) {
    type = value;
}

Expression* DataFieldSingle::get_initial_value() const {
    return initial_value;
}

void DataFieldSingle::set_initial_value(Expression* value) {
    initial_value = value;
}
