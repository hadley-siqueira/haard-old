#include "ast/named_type.h"

using namespace haard;

NamedType::NamedType() {
    kind = TYPE_NAMED;
    alias = nullptr;
    name = nullptr;
    symbol = nullptr;
}

NamedType::~NamedType() {
    delete alias;
    delete name;
}

Identifier* NamedType::get_alias() {
    return alias;
}

Identifier* NamedType::get_name() {
    return name;
}

Symbol* NamedType::get_symbol() {
    return symbol;
}

void NamedType::set_alias(Identifier* id) {
    alias = id;
}

void NamedType::set_name(Identifier* id) {
    name = id;
}

void NamedType::set_symbol(Symbol* symbol) {
    this->symbol = symbol;
}

std::string NamedType::to_cpp() {
    return symbol->to_cpp();
}

std::string NamedType::to_str() {
    return name->get_lexeme();
}
