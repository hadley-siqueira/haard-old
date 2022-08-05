#include "symtab/symbol.h"

using namespace haard;

Symbol::Symbol() {
    descriptor = nullptr;
}

Symbol::Symbol(int kind, std::string name, void* descriptor) {
    this->kind = kind;
    this->name = name;
    this->descriptor = descriptor;
}

int Symbol::get_kind() {
    return kind;
}

std::string Symbol::get_name() {
    return name;
}

void* Symbol::get_descriptor() {
    return descriptor;
}

void Symbol::set_kind(int kind) {
    this->kind = kind;
}

void Symbol::set_name(std::string name) {
    this->name = name;
}

void Symbol::set_descriptor(void* descriptor) {
    this->descriptor = descriptor;
}

