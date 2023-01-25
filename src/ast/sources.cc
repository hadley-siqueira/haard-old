#include "ast/sources.h"

using namespace haard;

Sources::Sources() {

}

Sources::~Sources() {
    for (int i = 0; i < sources.size(); ++i) {
        delete sources[i];
    }
}

void Sources::add_source(std::string path, Source* source) {
    sources_map[path] = source;
    sources.push_back(source);
}

Source* Sources::get_source(std::string path) {
    if (has_source(path)) {
        return sources_map[path];
    }

    return nullptr;
}

Source* Sources::get_source(int i) {
    if (i < sources.size()) {
        return sources[i];
    }

    return nullptr;
}

bool Sources::has_source(std::string path) {
    return sources_map.count(path) > 0;
}

int Sources::sources_count() {
    return sources.size();
}
