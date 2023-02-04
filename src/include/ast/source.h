#ifndef HAARD_AST_SOURCE_H
#define HAARD_AST_SOURCE_H

#include <string>
#include <vector>
#include "ast/import.h"
#include "ast/class.h"
#include "ast/data.h"
#include "ast/struct.h"
#include "ast/enum.h"
#include "ast/union.h"
#include "ast/function.h"
#include "ast/type_declaration.h"

namespace haard {
    class Import;
    class Scope;

    class Source {
        public:
            Source();
            ~Source();

        public:
            std::string get_path();
            Import* get_import(int idx);
            Import* get_import_with_alias(std::string alias);
            Function* get_function(int idx);
            Class* get_class(int idx);
            Data* get_data(int idx);
            Struct* get_struct(int idx);
            Enum* get_enum(int idx);
            Union* get_union(int idx);
            Scope* get_scope();

            void set_path(std::string path);
            void add_import(Import* import);
            void add_function(Function* function);
            void add_class(Class* klass);
            void add_data(Data* data);
            void add_struct(Struct* obj);
            void add_enum(Enum* obj);
            void add_union(Union* obj);
            
            int import_count();
            int function_count();
            int classes_count();
            int data_count();
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
            std::vector<Data*> datas;
            std::vector<Struct*> structs;
            std::vector<Enum*> enums;
            std::vector<Union*> unions;
            Scope* scope;
    };
}

#endif
