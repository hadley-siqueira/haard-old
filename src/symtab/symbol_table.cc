#include "symtab/symbol_table.h"

using namespace haard;

SymbolTable::SymbolTable() {
    parent = nullptr;
}

SymbolTable::~SymbolTable() {
    std::map<std::string, Symbol*>::iterator it;

    for (it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}
