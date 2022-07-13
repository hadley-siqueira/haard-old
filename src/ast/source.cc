#include "ast/source.h"

using namespace haard;

Source::Source() {

}

Source::~Source() {
    for (int i = 0; i < imports.size(); ++i) {
        delete imports[i];
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

void Source::set_path(std::string path) {
    this->path = path;
}
            
void Source::add_import(Import* import) {
    imports.push_back(import);
}
            
int Source::import_count() {
    return imports.size();
}
