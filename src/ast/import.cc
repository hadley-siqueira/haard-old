#include <sstream>
#include "ast/import.h"

using namespace haard;

Import::Import() {
    source = nullptr;
}

int Import::get_line() {
    return line;
}

int Import::get_column() {
    return column;
}

std::string Import::get_alias() {
    return alias;
}

std::vector<std::string> Import::get_path() {
    return path;
}
            
std::string Import::get_path(int idx) {
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

void Import::set_alias(std::string alias) {
    this->alias = alias;
}

void Import::set_path(std::vector<std::string> path) {
    this->path = path;
}

void Import::set_from_token(Token& token) {
    line = token.get_line();
    column = token.get_column();
}

void Import::set_source(Source* source) {
    this->source = source;
}

void Import::add_to_path(std::string s) {
    path.push_back(s);
}

bool Import::has_alias() {
    return alias.size() > 0;
}
            
int Import::path_count() {
    return path.size();
}
