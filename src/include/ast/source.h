#ifndef HAARD_AST_SOURCE_H
#define HAARD_AST_SOURCE_H

#include <string>
#include <vector>
#include "ast/import.h"
#include "ast/class.h"
#include "ast/function.h"

namespace haard {
    class Source {
        public:
            Source();
            ~Source();

        public:
            std::string get_path();
            Import* get_import(int idx);
            Function* get_function(int idx);
            Class* get_class(int idx);

            void set_path(std::string path);
            void add_import(Import* import);
            void add_function(Function* function);
            void add_class(Class* klass);
            
            int import_count();
            int function_count();
            int classes_count();

        private:
            std::string path;
            std::vector<Import*> imports;
            std::vector<Function*> functions;
            std::vector<Class*> classes;
    };
}

#endif
