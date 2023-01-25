#include <iostream>
#include "ast/variable.h"
#include "ast/named_type.h"

using namespace haard;

Variable::Variable() {
    line = 0;
    column = 0;
    type = nullptr;
}

Variable::Variable(Token& token) {
    name = token.get_lexeme();
    line = token.get_line();
    column = token.get_column();
    type = nullptr;
}

Variable::Variable(Identifier* id) {
    name = id->get_name();
    line = id->get_line();
    column = id->get_column();
    type = nullptr;
}

Variable::~Variable() {
    // delete type;
}

int Variable::get_line() {
    return line;
}

int Variable::get_column() {
    return column;
}

std::string Variable::get_name() {
    return name;
}

Type* Variable::get_type() {
    /*if (type->get_kind() == TYPE_NAMED) {
        NamedType* named = (NamedType*) type;

        if (named->get_symbol() && named->get_symbol()->get_kind() == SYM_TEMPLATE) {
            TemplateType* tt = (TemplateType*) named->get_symbol()->get_descriptor();
            return tt->get_bind_type();
        }
    }*/

    return type;
}

int Variable::get_size_in_bytes() {
    return type->get_size_in_bytes();
}

void Variable::set_line(int line) {
    this->line = line;
}

void Variable::set_column(int column) {
    this->column = column;
}

void Variable::set_name(std::string name) {
    this->name = name;
}

void Variable::set_type(Type* type) {
    this->type = type;
}

void Variable::set_from_token(Token& token) {
    line = token.get_line();
    column = token.get_column();
    name = token.get_lexeme();
}

int Variable::get_uid() {
    return uid;
}

void Variable::set_uid(int uid) {
    this->uid = uid;
}

void Variable::set_kind(int kind) {
    this->kind = kind;
}

int Variable::get_kind() {
    return kind;
}

std::string Variable::get_unique_name() {
    std::stringstream ss;

    switch (kind) {
    case VAR_LOCAL:
        ss << "v";
        break;

    case VAR_PARAM:
        ss << "p";
        break;

    case VAR_CLASS:
        ss << "cv";
        break;
    }

    ss << uid << '_' << name;
    return ss.str();
}

Variable* Variable::clone() {
    Variable* other = new Variable();

    other->uid = uid;
    other->line = line;
    other->column = column;
    other->constant = constant;
    other->name = name;

    if (type) {
        other->type = type->clone();
    }

    return other;
}

int Variable::get_offset() const
{
    return offset;
}

void Variable::set_offset(int value)
{
    offset = value;
}
