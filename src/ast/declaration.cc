#include "ast/declaration.h"

using namespace haard;

Declaration::Declaration() {

}

Declaration::~Declaration() {

}

int Declaration::get_kind() const {
    return kind;
}

void Declaration::set_kind(int newKind) {
    kind = newKind;
}

int Declaration::get_line() const {
    return line;
}

void Declaration::set_line(int newLine) {
    line = newLine;
}

int Declaration::get_column() const {
    return column;
}

void Declaration::set_column(int newColumn) {
    column = newColumn;
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
