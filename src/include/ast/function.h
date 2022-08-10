#ifndef HAARD_AST_FUNCTION_H
#define HAARD_AST_FUNCTION_H

#include <vector>
#include <string>
#include "token/token.h"
#include "ast/variable.h"
#include "ast/type.h"
#include "ast/compound_statement.h"

namespace haard {
    class Scope;

    class Function {
        public:
            Function();
            ~Function();

        public:
            int get_line();
            int get_column();
            const char* get_name();
            Variable* get_parameter(int idx);
            Variable* get_variable(int idx);
            Type* get_return_type();
            CompoundStatement* get_statements();
            Scope* get_scope();

            void set_line(int line);
            void set_column(int column);
            void set_name(const char* name);
            void set_from_token(Token& token);
            void set_return_type(Type* type);
            void set_statements(CompoundStatement* statements);

            int get_uid();
            void set_uid(int uid);

            Type* get_self_type();
            void set_self_type(Type* type);

            int parameters_count();

            void add_parameter(Variable* param);
            void add_variable(Variable* var);

        private:
            int uid;
            int line;
            int column;
            const char* name;
            std::vector<Variable*> parameters;
            std::vector<Variable*> variables;
            Type* return_type;
            Type* self_type;
            CompoundStatement* statements;
            Scope* scope;
            
    };
}

#endif
