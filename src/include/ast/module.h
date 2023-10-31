#ifndef HAARD_AST_MODULE_H
#define HAARD_AST_MODULE_H

#include <string>
#include <vector>
#include "ast/import.h"
#include "ast/class.h"
#include "ast/struct.h"
#include "ast/enum.h"
#include "ast/union.h"
#include "ast/function.h"
#include "ast/compound_type_descriptor.h"
#include "ast/declaration.h"

namespace haard {
    class Import;
    class Scope;

    class Module {
    public:
        Module();
        ~Module();

    public:
        std::string get_path();
        Import* get_import(int idx);
        Import* get_import_with_alias(std::string alias);
        Function* get_function(int idx);
        Class* get_class(int idx);
        Struct* get_struct(int idx);
        Enum* get_enum(int idx);
        Union* get_union(int idx);
        Declaration* get_declaration(int idx);
        Scope* get_scope();

        void set_path(std::string path);
        void add_import(Import* import);
        void add_function(Function* function);
        void add_class(Class* klass);
        void add_struct(Struct* obj);
        void add_enum(Enum* obj);
        void add_union(Union* obj);

        int import_count();
        int declarations_count();
        int functions_count();
        int classes_count();
        int structs_count();
        int enums_count();
        int unions_count();

        std::string get_relative_path() const;
        void set_relative_path(std::string value);

    private:
        std::string path;
        std::string relative_path;
        std::vector<Import*> imports;
        std::vector<Function*> functions;
        std::vector<Class*> classes;
        std::vector<Struct*> structs;
        std::vector<Enum*> enums;
        std::vector<Union*> unions;
        std::vector<Declaration*> declarations;
        Scope* scope;
    };
}

#endif
