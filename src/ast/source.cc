#include "ast/source.h"
#include "scope/scope.h"

using namespace haard;

Source::Source() {
    scope = new Scope();
}

Source::~Source() {
    for (int i = 0; i < imports.size(); ++i) {
        delete imports[i];
    }

    for (int i = 0; i < functions.size(); ++i) {
        delete functions[i];
    }

    for (int i = 0; i < classes.size(); ++i) {
        delete classes[i];
    }

    delete scope;
}

std::string Source::get_path() {
    return path;
}

Import* Source::get_import(int idx) {
    if (idx < imports.size()) {
        return imports[idx];
    }

    return nullptr;
}

Import* Source::get_import_with_alias(std::string alias) {
    Import* import = nullptr;

    for (int i = 0; i < import_count(); ++i) {
        import = get_import(i);

        if (alias == import->get_alias()) {
            return import;
        }
    }

    return nullptr;
}

Function* Source::get_function(int idx) {
    if (idx < functions.size()) {
        return functions[idx];
    }

    return nullptr;
}

Class* Source::get_class(int idx) {
    if (idx < classes_count()) {
        return classes[idx];
    }

    return nullptr;
}

Data* Source::get_data(int idx) {
    if (idx < data_count()) {
        return datas[idx];
    }

    return nullptr;
}

Scope* Source::get_scope() {
    return scope;
}

void Source::set_path(std::string path) {
    this->path = path;
}
            
void Source::add_import(Import* import) {
    imports.push_back(import);
}
            
void Source::add_function(Function* function) {
    functions.push_back(function);
    function->set_source(this);
}

void Source::add_class(Class* klass) {
    classes.push_back(klass);
    klass->set_source(this);
}

void Source::add_data(Data* data) {
    datas.push_back(data);
}

int Source::import_count() {
    return imports.size();
}

int Source::function_count() {
    return functions.size();
}

int Source::classes_count() {
    return classes.size();
}

int Source::data_count() {
    return datas.size();
}

std::string Source::get_relative_path() const {
    return relative_path;
}

void Source::set_relative_path(std::string value) {
    relative_path = value;
}
