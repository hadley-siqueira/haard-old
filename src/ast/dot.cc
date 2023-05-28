#include <sstream>
#include "ast/dot.h"

using namespace haard;

Dot::Dot(Expression* object, Expression* field) {
    this->kind = EXPR_DOT;
    this->object = object;
    this->field = field;
}

Dot::Dot(Token& token, Expression* object, Expression* field) {
    this->kind = EXPR_DOT;
    this->object = object;
    this->field = field;
    this->line = token.get_line();
    this->column = token.get_column();
}

Dot::~Dot() {
    delete object;
    delete field;
}

Expression* Dot::get_object() const {
    return object;
}

void Dot::set_object(Expression* value) {
    object = value;
}

Expression* Dot::get_field() const {
    return field;
}

void Dot::set_field(Expression* value) {
    field = value;
}

std::string Dot::to_str() {
    std::stringstream ss;

    ss << object->to_str();
    ss << ".";
    ss << field->to_str();

    return ss.str();
}
