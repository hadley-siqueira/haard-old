#include "ast/declaration.h"
#include "defs.h"

using namespace haard;

Declaration::Declaration() {
    template_header = nullptr;
}

Declaration::~Declaration() {
    delete template_header;
}

const std::string& Declaration::get_name() const {
    return name;
}

void Declaration::set_name(const std::string& newName) {
    name = newName;
}

Module* Declaration::get_module() const {
    return module;
}

void Declaration::set_module(Module* newModule) {
    module = newModule;
}

TemplateHeader* Declaration::get_template_header() {
    return template_header;
}

void Declaration::set_template_header(TemplateHeader* header, bool is_template) {
    template_header = header;
    template_header->set_template_flag(is_template);
}

bool Declaration::is_type_declaration() {
    int kind = get_kind();

    return kind == DECL_CLASS
        || kind == DECL_STRUCT
        || kind == DECL_ENUM
        || kind == DECL_UNION;
}

bool Declaration::is_function() {
    int kind = get_kind();

    return kind == DECL_FUNCTION || kind == DECL_METHOD;
}
