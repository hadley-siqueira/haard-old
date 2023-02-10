#include <sstream>
#include "ast/template_header.h"

using namespace haard;

TemplateHeader::TemplateHeader() {
    types = new TypeList();
}

TemplateHeader::~TemplateHeader() {
    delete types;
}

void TemplateHeader::add_type(Type* type) {
    types->add_type(type);
}

Type* TemplateHeader::get_type(int idx) {
    return types->get_type(idx);
}

int TemplateHeader::types_count() {
    return types->types_count();
}

TypeList* TemplateHeader::get_types() {
    return types;
}

TemplateHeader* TemplateHeader::clone() {
    return nullptr;
}

std::string TemplateHeader::to_str() {
    std::stringstream ss;

    ss << "<";
    ss << types->to_str();
    ss << ">";

    return ss.str();
}

std::string TemplateHeader::get_qualified_name() {
    std::stringstream ss;

    ss << "<";
    ss << types->get_qualified_name();
    ss << ">";

    return ss.str();
}
