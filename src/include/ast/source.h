#ifndef HAARD_AST_SOURCE_H
#define HAARD_AST_SOURCE_H

#include <string>
#include <vector>
#include "ast/import.h"
#include "ast/class.h"
#include "ast/function.h"

namespace haard {
    class Import;
    class Scope;

    class Source {
        public:
            Source();
            ~Source();

        public:
            const char* get_path();
            Import* get_import(int idx);
            Import* get_import_with_alias(std::string alias);
            Function* get_function(int idx);
            Class* get_class(int idx);
            Scope* get_scope();

            void set_path(const char* path);
            void add_import(Import* import);
            void add_function(Function* function);
            void add_class(Class* klass);
            
            int import_count();
            int function_count();
            int classes_count();

            const char* get_relative_path() const;
            void set_relative_path(const char* value);

    private:
            const char* path;
            const char* relative_path;
            std::vector<Import*> imports;
            std::vector<Function*> functions;
            std::vector<Class*> classes;
            Scope* scope;
    };
}

#endif
