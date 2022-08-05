#include "ast/named_type.h"

using namespace haard;

NamedType::NamedType() {
    kind = TYPE_NAMED;
    alias = nullptr;
    name = nullptr;
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

void NamedType::set_alias(Identifier* id) {
    alias = id;
}

void NamedType::set_name(Identifier* id) {
    name = id;
}
