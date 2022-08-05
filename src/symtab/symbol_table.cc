#include "symtab/symbol_table.h"

using namespace haard;

SymbolTable::SymbolTable() {
    parent = nullptr;
}

SymbolTable::~SymbolTable() {
    std::map<const char*, Symbol*>::iterator it;

    for (it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}

SymbolTable* SymbolTable::get_parent() {
    return parent;
}

void SymbolTable::set_parent(SymbolTable* symtab) {
    parent = symtab;
}

void SymbolTable::define(Class* klass) {
    Symbol* sym = new Symbol(SYM_CLASS, klass->get_name(), klass);
    symbols[klass->get_name()] = sym;
}

void SymbolTable::define(Function* func) {
    Symbol* sym = new Symbol(SYM_FUNCTION, func->get_name(), func);
    symbols[func->get_name()] = sym;
}

bool SymbolTable::has_parent() {
    return parent != nullptr;
}

Symbol* SymbolTable::has(const char* name) {
    if (symbols.count(name) > 0) {
        return symbols[name];
    }

    if (has_parent()) {
        return parent->has_class(name);
    }

    return nullptr;
}

Symbol* SymbolTable::has_class(const char* name) {
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
