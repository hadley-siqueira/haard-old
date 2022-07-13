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

Function* Source::get_function(int idx) {
    if (idx < functions.size()) {
        return functions[idx];
    }

    return nullptr;
}

void Source::set_path(std::string path) {
    this->path = path;
}
            
void Source::add_import(Import* import) {
    imports.push_back(import);
}
            
void Source::add_function(Function* function) {
    functions.push_back(function);
}

int Source::import_count() {
    return imports.size();
}

int Source::function_count() {
    return functions.size();
}
