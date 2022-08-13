#include "ast/template_type.h"

using namespace haard;

TemplateType::TemplateType() {
    kind = TYPE_TEMPLATE;
    name = nullptr;
}

TemplateType::TemplateType(Token& token) {
    kind = TYPE_TEMPLATE;
    name = token.get_lexeme();
}

const char* TemplateType::get_name() {
    return name;
}

void TemplateType::set_name(const char* name) {
    this->name = name;
}

std::string TemplateType::to_str() {
    return name;
}

std::string TemplateType::to_cpp() {
    return name;
}

Type* TemplateType::clone() {
    TemplateType* other = new TemplateType();

    other->name = name;

    return other;
}
