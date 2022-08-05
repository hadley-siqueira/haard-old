#include "scope/symbol.h"

using namespace haard;

Symbol::Symbol() {
    name = nullptr;
}

Symbol::Symbol(int kind, const char* name, void* descriptor) {
    this->kind = kind;
    this->name = name;
    descriptors.push_back(descriptor);
}

int Symbol::get_kind() {
    return kind;
}

const char* Symbol::get_name() {
    return name;
}

void* Symbol::get_descriptor(int idx) {
    if (idx < descriptors.size()) {
        return descriptors[idx];
    }

    return nullptr;
}

void Symbol::set_kind(int kind) {
    this->kind = kind;
}

void Symbol::set_name(const char* name) {
    this->name = name;
}

void Symbol::add_descriptor(void* descriptor) {
    descriptors.push_back(descriptor);
}

