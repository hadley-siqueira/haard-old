#include <iostream>
#include "ast/indirection_type.h"

using namespace haard;

IndirectionType::IndirectionType(int kind, Token& token, Type* subtype) {
    this->kind = kind;
    this->subtype = subtype;

    line = token.get_line();
    column = token.get_column();
}

IndirectionType::IndirectionType(int kind, Type* subtype) {
    this->kind = kind;
    this->subtype = subtype;
}

IndirectionType::~IndirectionType() {
    // delete subtype;
}

Type* IndirectionType::get_subtype() {
    return subtype;
}

bool IndirectionType::equal(Type* type) {
    IndirectionType* other = (IndirectionType*) type;

    if (kind == other->get_kind()) {
        return subtype->equal(other->get_subtype());
    } else if (kind == TYPE_PARENTHESIS) {
        return subtype->equal(type);
    }

    return false;
}

Type* IndirectionType::clone() {
    return new IndirectionType(kind, subtype->clone());
}

std::string IndirectionType::to_str() {
    if (kind == TYPE_POINTER) {
        return subtype->to_str() + "*";
    }

    return subtype->to_str() + "&";
}

std::string IndirectionType::to_cpp() {
    if (kind == TYPE_POINTER) {
        return subtype->to_cpp() + "*";
    }

    return subtype->to_cpp() + "&";
}

std::string IndirectionType::get_qualified_name() {
    if (kind == TYPE_POINTER) {
        return subtype->get_qualified_name() + "*";
    }

    return subtype->get_qualified_name() + "&";
}

bool IndirectionType::is_integer_scalar() {
    return true;
}

int IndirectionType::get_size_in_bytes() {
    return ARCH_WORD_SIZE;
}
