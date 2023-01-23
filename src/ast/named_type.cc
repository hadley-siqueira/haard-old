#include <iostream>
#include "ast/ast.h"
#include "ast/named_type.h"
#include "utils/string_pool.h"

using namespace haard;

NamedType::NamedType() {
    kind = TYPE_NAMED;
    alias = nullptr;
    name = nullptr;
    symbol = nullptr;
    template_header = nullptr;
}

NamedType::~NamedType() {
    /* Empty */
}

const char* NamedType::get_alias() {
    return alias;
}

const char* NamedType::get_name() {
    return name;
}

Symbol* NamedType::get_symbol() {
    return symbol;
}

void NamedType::set_alias(const char* id) {
    alias = id;
}

void NamedType::set_name(const char* id) {
    name = StringPool::get(id);
}

void NamedType::set_symbol(Symbol* symbol) {
    this->symbol = symbol;
}

std::string NamedType::to_cpp() {
    return symbol->to_cpp();
}

std::string NamedType::to_str() {
    return name;
}

bool NamedType::equal(Type* type) {
    if (type->get_kind() != TYPE_NAMED) {
        return false;
    }

    NamedType* other = (NamedType*) type;
    return symbol->get_descriptor() == other->symbol->get_descriptor();
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

    other->alias = alias;
    other->name = name;

    return other;
}

void NamedType::set_template_header(TypeList* header) {
    template_header = header;
}

TypeList* NamedType::get_template_header() {
    return template_header;
}

int NamedType::get_size_in_bytes() {
    return symbol->get_size_in_bytes();
}

int NamedType::get_alignment() {
    return symbol->get_alignment();
}

Scope* NamedType::get_scope() {
    return symbol->get_descriptor_scope();
}

bool NamedType::is_class() {
    return kind == TYPE_NAMED && symbol->get_kind() == SYM_CLASS;
}
