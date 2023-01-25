#include <iostream>
#include <sstream>
#include "ast/template_type.h"

using namespace haard;

TemplateType::TemplateType() {
    kind = TYPE_TEMPLATE;
    name = nullptr;
    bind_type = nullptr;
}

TemplateType::TemplateType(Token& token) {
    kind = TYPE_TEMPLATE;
    name = token.get_lexeme();
    bind_type = nullptr;
}

std::string TemplateType::get_name() {
    return name;
}

void TemplateType::set_name(std::string name) {
    this->name = name;
}

std::string TemplateType::to_str() {
    std::stringstream ss;

    ss << name;

    if (is_binded()) {
        ss << ":" << bind_type->to_str();
    }

    return ss.str();
}

std::string TemplateType::to_cpp() {
    return name;
}

std::string TemplateType::get_qualified_name() {
    std::stringstream ss;

    ss << name;

    if (is_binded()) {
        ss << ":" << bind_type->get_qualified_name();
    }

    return ss.str();
}

bool TemplateType::equal(Type* type) {
    if (is_binded()) {
        return bind_type->equal(type);
    }

    return false;
}

int TemplateType::get_size_in_bytes() {
    if (is_binded()) {
        return bind_type->get_size_in_bytes();
    }

    return 1;
}

int TemplateType::get_alignment() {
    if (is_binded()) {
        return bind_type->get_alignment();
    }

    return 8;
}

bool TemplateType::is_binded() {
    return bind_type != nullptr;
}

Type* TemplateType::clone() {
    TemplateType* other = new TemplateType();

    other->name = name;

    return other;
}

Type* TemplateType::get_bind_type() {
    return bind_type;
}

void TemplateType::set_bind_type(Type* type) {
    bind_type = type;
}
