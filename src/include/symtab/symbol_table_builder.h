#ifndef HAARD_SYMBOL_TABLE_BUILDER_H
#define HAARD_SYMBOL_TABLE_BUILDER_H

#include "ast/ast.h"
#include "symtab/symbol_table.h"

namespace haard {
    class SymbolTableBuilder {
        public:
            SymbolTableBuilder();

        public:
            void build_sources(Sources* sources);
            void build_source(Source* source);

            void build_class(Class* klass);

        private:
            int pass;
            SymbolTable* current_table;
    };
}

#endif
