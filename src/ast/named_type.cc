#include <iostream>
#include "ast/ast.h"
#include "ast/named_type.h"

using namespace haard;

NamedType::NamedType() {
    kind = TYPE_NAMED;
    id = new Identifier();
}

NamedType::NamedType(Token& token) {
    kind = TYPE_NAMED;
    id = new Identifier(token);
}

NamedType::~NamedType() {
    delete id;
}

std::string NamedType::get_alias() {
    return id->get_alias();
}

std::string NamedType::get_name() {
    return id->get_name();
}

Symbol* NamedType::get_symbol() {
    return id->get_symbol();
}

void NamedType::set_alias(std::string id) {
    this->id->set_alias(id);
}

void NamedType::set_name(std::string id) {
    this->id->set_name(id);
}

void NamedType::set_symbol(Symbol* symbol) {
    this->id->set_symbol(symbol);
}

std::string NamedType::to_str() {
    return id->to_str();
}

bool NamedType::equal(Type* type) {
    if (type->get_kind() != TYPE_NAMED) {
        return false;
    }

    NamedType* other = (NamedType*) type;
    return id->get_symbol()->get_descriptor() == other->get_symbol()->get_descriptor();
}

bool NamedType::is_integer_scalar() {
    return false;
}

Symbol* NamedType::has_field(std::string name) {
    Class* klass;
    Symbol* sym = nullptr;

    if (!id->get_symbol()) {
        return nullptr;
    }

    switch (id->get_symbol()->get_kind()) {
    case SYM_CLASS:
        klass = (Class*) id->get_symbol()->get_descriptor();
        sym = klass->get_scope()->has_field(name);
        break;

    default:
        std::cout << __FILE__ << ' ' << __LINE__ << " invalid symbol kind\n";
        exit(0);
    }

    return sym;
}

void NamedType::set_template_header(TemplateHeader* header) {
    id->set_template_header(header);
}

TemplateHeader *NamedType::get_template_header() {
    return id->get_template_header();
}

int NamedType::get_size_in_bytes() {
    return id->get_symbol()->get_size_in_bytes();
}

int NamedType::get_alignment() {
    return id->get_symbol()->get_alignment();
}

Scope* NamedType::get_scope() {
    return id->get_symbol()->get_descriptor_scope();
}

bool NamedType::is_class() {
    return kind == TYPE_NAMED && id->get_symbol()->get_kind() == SYM_CLASS;
}

std::string NamedType::get_qualified_name() {
    return id->get_symbol()->get_qualified_name();
}
