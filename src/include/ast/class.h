#ifndef HAARD_AST_CLASS_H
#define HAARD_AST_CLASS_H

#include <vector>
#include "token/token.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/type.h"

namespace haard {
    class Scope;

    class Class {
        public:
            Class();
            ~Class();

        public:
            const char* get_name();
            int get_line();
            int get_column();
            Function* get_method(int idx);
            Variable* get_variable(int idx);
            Type* get_parent();
            Scope* get_scope();

            void set_from_token(Token& token);
            void set_name(const char* name);
            void set_line(int line);
            void set_column(int column);
            void set_parent(Type* type);

            void add_method(Function* method);
            void add_variable(Variable* var);

            int methods_count();
            int variables_count();

        private:
            const char* name;
            int line;
            int column;
            Type* parent;
            std::vector<Function*> methods;
            std::vector<Variable*> variables;
            Scope* scope;
    };
}

#endif
