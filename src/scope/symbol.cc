#include "scope/symbol.h"
#include "defs.h"

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

std::string Symbol::to_str() {
    std::stringstream ss;

    ss << "<" << name << ":";

    switch (kind) {
    case SYM_CLASS:
        ss << "class";
        break;

    case SYM_FUNCTION:
        ss << "func";
        break;

    case SYM_PARAMETER:
        ss << "param";
        break;

    case SYM_VARIABLE:
        ss << "var";
        break;

    case SYM_CLASS_VARIABLE:
        ss << "cvar";
        break;
    }

    ss << ">";
    return ss.str();
}
