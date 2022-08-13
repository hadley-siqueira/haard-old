#include <iostream>
#include "ast/ast.h"
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

Symbol* NamedType::has_field(const char* name) {
    Class* klass;
    Symbol* sym = nullptr;

    if (!symbol) {
        return nullptr;
    }

    switch (symbol->get_kind()) {
    case SYM_CLASS:
        klass = (Class*) symbol->get_descriptor();
        sym = klass->get_scope()->has_field(name);
        break;

    default:
        std::cout << __FILE__ << ' ' << __LINE__ << " invalid symbol kind\n";
        exit(0);
    }

    return sym;
}

Type* NamedType::clone() {
    NamedType* other = new NamedType();

    if (alias) {
        other->alias = (Identifier*) alias->clone();
    }

    if (name) {
        other->name = (Identifier*) name->clone();
    }

    return other;
}

