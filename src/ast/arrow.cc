#include <sstream>
#include "ast/arrow.h"

using namespace haard;

Arrow::Arrow(Expression* object, Expression* field) {
    this->kind = EXPR_ARROW;
    this->object = object;
    this->field = field;
}

Arrow::Arrow(Token& token, Expression* object, Expression* field) {
    this->kind = EXPR_ARROW;
    this->object = object;
    this->field = field;
    this->line = token.get_line();
    this->column = token.get_column();
}

Arrow::~Arrow() {
    delete object;
    delete field;
}

Expression* Arrow::get_object() const {
    return object;
}

void Arrow::set_object(Expression* value) {
    object = value;
}

Expression* Arrow::get_field() const {
    return field;
}

void Arrow::set_field(Expression* value) {
    field = value;
}

std::string Arrow::to_str() {
    std::stringstream ss;

    ss << object->to_str();
    ss << "->";
    ss << field->to_str();

    return ss.str();
}
