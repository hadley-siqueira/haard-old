#include "ast/module.h"
#include "scope/scope.h"

using namespace haard;

Module::Module() {
    scope = new Scope();
}

Module::~Module() {
    for (int i = 0; i < import_count(); ++i) {
        delete imports[i];
    }

    for (int i = 0; i < declarations_count(); ++i) {
        delete declarations[i];
    }

    delete scope;
}

std::string Module::get_path() {
    return path;
}

Import* Module::get_import(int idx) {
    if (idx < imports.size()) {
        return imports[idx];
    }

    return nullptr;
}

Import* Module::get_import_with_alias(std::string alias) {
    Import* import = nullptr;

    for (int i = 0; i < import_count(); ++i) {
        import = get_import(i);

        if (alias == import->get_alias()) {
            return import;
        }
    }

    return nullptr;
}

Function* Module::get_function(int idx) {
    if (idx < functions.size()) {
        return functions[idx];
    }

    return nullptr;
}

Class* Module::get_class(int idx) {
    if (idx < classes_count()) {
        return classes[idx];
    }

    return nullptr;
}

Struct* Module::get_struct(int idx) {
    if (idx < structs_count()) {
        return structs[idx];
    }

    return nullptr;
}

Enum* Module::get_enum(int idx) {
    if (idx < enums_count()) {
        return enums[idx];
    }

    return nullptr;
}

Union* Module::get_union(int idx) {
    if (idx < unions_count()) {
        return unions[idx];
    }

    return nullptr;
}

Declaration *Module::get_declaration(int idx) {
    if (idx < declarations_count()) {
        return declarations[idx];
    }

    return nullptr;
}

Scope* Module::get_scope() {
    return scope;
}

void Module::set_path(std::string path) {
    this->path = path;
}
            
void Module::add_import(Import* import) {
    imports.push_back(import);
}
            
void Module::add_function(Function* function) {
    functions.push_back(function);
    declarations.push_back(function);
    function->set_module(this);
    function->get_scope()->set_parent(get_scope());
}

void Module::add_class(Class* klass) {
    classes.push_back(klass);
    declarations.push_back(klass);
    klass->set_module(this);
}

void Module::add_struct(Struct* obj) {
    structs.push_back(obj);
    declarations.push_back(obj);
    obj->set_module(this);
}

void Module::add_enum(Enum* obj) {
    enums.push_back(obj);
    declarations.push_back(obj);
    obj->set_module(this);
}

void Module::add_union(Union* obj) {
    unions.push_back(obj);
    declarations.push_back(obj);
    obj->set_module(this);
}

int Module::import_count() {
    return imports.size();
}

int Module::declarations_count() {
    return declarations.size();
}

int Module::functions_count() {
    return functions.size();
}

int Module::classes_count() {
    return classes.size();
}

int Module::structs_count() {
    return structs.size();
}

int Module::enums_count() {
    return enums.size();
}

int Module::unions_count() {
    return unions.size();
}

std::string Module::get_relative_path() const {
    return relative_path;
}

void Module::set_relative_path(std::string value) {
    relative_path = value;
}
