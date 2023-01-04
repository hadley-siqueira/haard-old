#ifndef HAARD_AST_FUNCTION_H
#define HAARD_AST_FUNCTION_H

#include <vector>
#include <string>
#include "token/token.h"
#include "ast/variable.h"
#include "ast/type.h"
#include "ast/type_list.h"
#include "ast/function_type.h"
#include "ast/template_header.h"
#include "ast/compound_statement.h"

namespace haard {
    class Scope;
    class Class;
    class Source;

    class Function {
        public:
            Function();
            ~Function();

        public:
            int get_line();
            int get_column();
            const char* get_name();
            std::string get_cpp_name();
            std::string get_cpp_signature();
            std::string get_type_signature();
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
            void set_virtual(bool v);

            int get_uid();
            void set_uid(int uid);

            void set_method(bool value=true);

            Type* get_self_type();
            void set_self_type(FunctionType* type);

            int parameters_count();

            void add_parameter(Variable* param);
            void add_variable(Variable* var);

            bool is_template();
            bool is_method();
            bool is_virtual();
            bool is_binded_with_types(TypeList* types);
            void bind_with(TypeList* types);

            Function* clone();
            Function* get_with_template_binding(TypeList* bindings);
            bool same_signature(Function* other);

            void set_source(Source* source);
            Source* get_source();

            void set_overloaded_index(int idx);
            int get_overloaded_index();

            void set_template_header(TemplateHeader* header);
            TemplateHeader* get_template_header();

            void set_class(Class* klass);
            Class* get_class();

            std::vector<std::string> get_annotations() const;
            void set_annotations(const std::vector<std::string>& value);

            Function* get_parent_method() const;
            void set_parent_method(Function* value);

    private:
            int uid;
            int line;
            int column;
            int overloaded_index;
            bool method_flag;
            bool virtual_flag;
            const char* name;
            Type* return_type;
            FunctionType* self_type;
            TemplateHeader* template_header;
            CompoundStatement* statements;
            Function* parent_method;
            Scope* scope;
            Source* source;
            Class* klass;
            std::vector<Variable*> parameters;
            std::vector<Variable*> variables;
            std::vector<std::string> annotations;
            
    };
}

#endif
