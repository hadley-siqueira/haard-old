#ifndef HAARD_AST_CLASS_H
#define HAARD_AST_CLASS_H

#include <vector>
#include "token/token.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/type.h"
#include "ast/named_type.h"

namespace haard {
    class Scope;
    class Source;

    class Class {
        public:
            Class();
            ~Class();

        public:
            const char* get_name();
            std::string get_cpp_name();
            int get_line();
            int get_column();
            int get_uid();
            Function* get_method(int idx);
            Variable* get_variable(int idx);
            Type* get_parent();
            Scope* get_scope();
            NamedType* get_self_type();

            void set_from_token(Token& token);
            void set_name(const char* name);
            void set_line(int line);
            void set_column(int column);
            void set_uid(int uid);
            void set_parent(Type* type);
            void set_self_type(NamedType* type);

            void add_method(Function* method);
            void add_variable(Variable* var);

            int methods_count();
            int variables_count();

            int constructors_count();
            Function* get_constructor(int idx);

            Source* get_source();
            void set_source(Source* source);

        private:
            const char* name;
            int line;
            int column;
            int uid;
            Type* parent;
            std::vector<Function*> methods;
            std::vector<Function*> constructors;
            Function* destructor;
            std::vector<Variable*> variables;
            Scope* scope;
            NamedType* self_type;
            Source* source; // source where this class is defined
    };
}

#endif
