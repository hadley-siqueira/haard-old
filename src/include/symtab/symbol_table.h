#ifndef HAARD_SYMBOL_TABLE_H
#define HAARD_SYMBOL_TABLE_H

#include <map>
#include "symtab/symbol.h"

namespace haard {
    class SymbolTable {
        public:
            SymbolTable();
            ~SymbolTable();

        public:

        private:
            std::map<std::string, Symbol*> symbols;
            SymbolTable* parent;
    };
}

#endif
