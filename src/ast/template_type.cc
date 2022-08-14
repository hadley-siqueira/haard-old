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

const char* TemplateType::get_name() {
    return name;
}

void TemplateType::set_name(const char* name) {
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
