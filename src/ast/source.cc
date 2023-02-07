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

Struct* Source::get_struct(int idx) {
    if (idx < structs_count()) {
        return structs[idx];
    }

    return nullptr;
}

Enum* Source::get_enum(int idx) {
    if (idx < enums_count()) {
        return enums[idx];
    }

    return nullptr;
}

Union* Source::get_union(int idx) {
    if (idx < unions_count()) {
        return unions[idx];
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
    function->get_scope()->set_parent(get_scope());
}

void Source::add_class(Class* klass) {
    classes.push_back(klass);
    klass->set_source(this);
}

void Source::add_data(Data* data) {
    datas.push_back(data);
    data->set_source(this);
}

void Source::add_struct(Struct* obj) {
    structs.push_back(obj);
    obj->set_source(this);
}

void Source::add_enum(Enum* obj) {
    enums.push_back(obj);
    obj->set_source(this);
}

void Source::add_union(Union* obj) {
    unions.push_back(obj);
    obj->set_source(this);
}

int Source::import_count() {
    return imports.size();
}

int Source::functions_count() {
    return functions.size();
}

int Source::classes_count() {
    return classes.size();
}

int Source::data_count() {
    return datas.size();
}

int Source::structs_count() {
    return structs.size();
}

int Source::enums_count() {
    return enums.size();
}

int Source::unions_count() {
    return unions.size();
}

std::string Source::get_relative_path() const {
    return relative_path;
}

void Source::set_relative_path(std::string value) {
    relative_path = value;
}
