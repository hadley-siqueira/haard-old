#include <iostream>
#include "scope/scope.h"

using namespace haard;

Scope::Scope() {
    parent = nullptr;
}

Scope::~Scope() {
    std::map<const char*, Symbol*>::iterator it;

    for (it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}

Scope* Scope::get_parent() {
    return parent;
}

void Scope::set_parent(Scope* symtab) {
    parent = symtab;
}

Symbol* Scope::define(Class* klass) {
    Symbol* sym = new Symbol(SYM_CLASS, klass->get_name(), klass);
    symbols[klass->get_name()] = sym;
    return sym;
}

Symbol* Scope::define(Function* func) {
    Symbol* sym = new Symbol(SYM_FUNCTION, func->get_name(), func);
    symbols[func->get_name()] = sym;
    return sym;
}

Symbol* Scope::define(int kind, Function* func) {
    Symbol* sym = new Symbol(kind, func->get_name(), func);
    symbols[func->get_name()] = sym;
    return sym;
}

Symbol* Scope::define(int kind, Variable* var) {
    Symbol* sym = new Symbol(kind, var->get_name(), var);
    symbols[var->get_name()] = sym;
    return sym;
}

bool Scope::has_parent() {
    return parent != nullptr;
}

Symbol* Scope::has(const char* name) {
    if (symbols.count(name) > 0) {
        return symbols[name];
    }

    if (has_parent()) {
        return parent->has(name);
    }

    return nullptr;
}

Symbol* Scope::has_class(const char* name) {
    Symbol* sym = nullptr;

    if (symbols.count(name) > 0) {
        sym = symbols[name];

        if (sym->get_kind() == SYM_CLASS) {
            return sym;
        }
    }

    if (has_parent()) {
        return parent->has_class(name);
    }

    return nullptr;
}

void Scope::debug() {
    Symbol* sym;
    std::map<const char*, Symbol*>::iterator it;

    if (has_parent()) {
        parent->debug();
        std::cout << " -> ";
    }

    std::cout << "{";

    for (it = symbols.begin(); it != symbols.end(); ++it) {
        sym = it->second;
        std::cout << sym->to_str() << ", ";
    }

    std::cout << "}";
}
