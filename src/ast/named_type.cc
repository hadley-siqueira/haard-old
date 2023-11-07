#include <iostream>
#include "ast/ast.h"
#include "ast/named_type.h"
#include "scope/scope.h"

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

SymbolDescriptor* NamedType::get_symbol_descriptor() {
    return id->get_symbol_descriptor();
}

void NamedType::set_alias(std::string id) {
    this->id->set_alias(id);
}

void NamedType::set_name(std::string id) {
    this->id->set_name(id);
}

void NamedType::set_symbol_descriptor(SymbolDescriptor* value) {
    this->id->set_symbol_descriptor(value);
}

std::string NamedType::to_str() {
    return id->to_str();
}

bool NamedType::equal(Type* type) {
    if (type->get_kind() != TYPE_NAMED) {
        return false;
    }

    NamedType* other = (NamedType*) type;
    return id->get_symbol_descriptor()->get_descriptor() == other->get_symbol_descriptor()->get_descriptor();
}

bool NamedType::is_integer_scalar() {
    return false;
}

void NamedType::set_template_header(TemplateHeader* header) {
    id->set_template_header(header);
}

TemplateHeader* NamedType::get_template_header() {
    return id->get_template_header();
}

int NamedType::get_size_in_bytes() {
    return id->get_symbol_descriptor()->get_size_in_bytes();
}

int NamedType::get_alignment() {
    return id->get_symbol_descriptor()->get_alignment();
}

Scope* NamedType::get_scope() {
    return id->get_symbol_descriptor()->get_descriptor_scope();
}

bool NamedType::is_class() {
    return kind == TYPE_NAMED && id->get_symbol_descriptor()->get_kind() == SYM_CLASS;
}

std::string NamedType::get_qualified_name() {
    return id->get_symbol_descriptor()->get_qualified_name();
}
