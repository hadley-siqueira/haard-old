#ifndef HAARD_AST_SOURCE_H
#define HAARD_AST_SOURCE_H

#include <string>
#include <vector>
#include "ast/import.h"
#include "ast/class.h"
#include "ast/function.h"

namespace haard {
    class Import;
    class SymbolTable;

    class Source {
        public:
            Source();
            ~Source();

        public:
            const char* get_path();
            Import* get_import(int idx);
            Function* get_function(int idx);
            Class* get_class(int idx);
            SymbolTable* get_symbol_table();

            void set_path(const char* path);
            void add_import(Import* import);
            void add_function(Function* function);
            void add_class(Class* klass);
            
            int import_count();
            int function_count();
            int classes_count();

        private:
            const char* path;
            std::vector<Import*> imports;
            std::vector<Function*> functions;
            std::vector<Class*> classes;
            SymbolTable* symtab;
    };
}

#endif
