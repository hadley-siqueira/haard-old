#include <iostream>
#include <fstream>
#include <regex>
#include "ast/declaration.h"
#include "ast/module.h"
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

bool Declaration::is_template() {
    if (get_template_header()) {
        return get_template_header()->is_template();
    }

    return false;
}

std::string Declaration::get_with_templates(TypeList* types) {
    std::string body = get_original();
    TypeList* ttypes = get_template_header()->get_types();

    for (int i = 0; i < ttypes->types_count(); ++i) {
        std::string from = ttypes->get_type(i)->to_str();
        std::string to = types->get_type(i)->to_str();
        std::regex pattern("\\b" + from + "\\b");
        body = std::regex_replace(body, pattern, to);
    }

    std::cout << body << '\n';
    DBG;exit(0);
}

std::string Declaration::get_original() {
    char c;
    std::ifstream file;
    std::string buffer;
    int counter;

    file.open(get_path());

    file.seekg(get_begin());
    counter = get_begin();

    while (counter < get_end() && file.get(c)) {
        buffer += c;
        ++counter;
    }

    buffer += "\n";

    file.close();
    return buffer;
}

std::string Declaration::get_path() {
    return module->get_path();
}
