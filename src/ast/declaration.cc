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

Declaration* Declaration::get_templates(TemplateHeader* types) {
    if (types == nullptr && get_template_header() == nullptr) {
        return this;
    }

    if (types == nullptr && get_template_header() != nullptr) {
        return nullptr;
    }

    if (types != nullptr && get_template_header() == nullptr) {
        return nullptr;
    }

    if (types->types_count() != get_template_header()->types_count()) {
        return nullptr;
    }

    for (int i = 0; i < instances.size(); ++i) {
        TemplateHeader* th = instances[i]->get_template_header();
        bool found = true;

        for (int i = 0; i < th->types_count(); ++i) {
            if (!th->get_type(i)->equal(types->get_type(i))) {
                found = false;
            }
        }

        if (found) {
            return instances[i];
        }
    }

    return nullptr;
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

    return body;
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

void Declaration::add_instance(Declaration* decl) {
    instances.push_back(decl);
}
