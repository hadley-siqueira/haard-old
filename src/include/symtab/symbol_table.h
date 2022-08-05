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
            void define(Function* func);

            bool has_parent();
            Symbol* has(const char* name);
            Symbol* has_class(const char* name);

        private:
            std::map<const char*, Symbol*> symbols;
            SymbolTable* parent;
    };
}

#endif
