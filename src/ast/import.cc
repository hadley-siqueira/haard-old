#include <sstream>
#include "ast/import.h"

using namespace haard;

Import::Import() {
    source = nullptr;
    alias = nullptr;
}

int Import::get_line() {
    return line;
}

int Import::get_column() {
    return column;
}

const char* Import::get_alias() {
    return alias;
}

std::vector<const char*> Import::get_path() {
    return path;
}
            
const char* Import::get_path(int idx) {
    return path[idx];
}

Source* Import::get_source() {
    return source;
}

void Import::set_line(int line) {
    this->line = line;
}

void Import::set_column(int column) {
    this->column = column;
}

void Import::set_alias(const char* alias) {
    this->alias = alias;
}

void Import::set_path(std::vector<const char*> path) {
    this->path = path;
}

void Import::set_from_token(Token& token) {
    line = token.get_line();
    column = token.get_column();
}

void Import::set_source(Source* source) {
    this->source = source;
}

void Import::add_to_path(const char* s) {
    path.push_back(s);
}

bool Import::has_alias() {
    return alias != nullptr;
}

int Import::path_count() {
    return path.size();
}
