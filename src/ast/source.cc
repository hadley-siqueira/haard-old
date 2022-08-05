#include "ast/source.h"

using namespace haard;

Source::Source() {

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
}

const char* Source::get_path() {
    return path;
}

Import* Source::get_import(int idx) {
    if (idx < imports.size()) {
        return imports[idx];
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

void Source::set_path(const char* path) {
    this->path = path;
}
            
void Source::add_import(Import* import) {
    imports.push_back(import);
}
            
void Source::add_function(Function* function) {
    functions.push_back(function);
}

void Source::add_class(Class* klass) {
    classes.push_back(klass);
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
