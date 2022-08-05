#ifndef HAARD_SYMBOL_TABLE_H
#define HAARD_SYMBOL_TABLE_H

#include <map>
#include "ast/ast.h"
#include "symtab/symbol.h"

namespace haard {
    class SymbolTable {
        public:
            SymbolTable();
            ~SymbolTable();

        public:
            SymbolTable* get_parent();

            void set_parent(SymbolTable* symtab);

            void define(Class* klass);

            bool has_parent();
            Symbol* has_class(std::string& name);

        private:
            std::map<std::string, Symbol*> symbols;
            SymbolTable* parent;
    };
}

#endif
