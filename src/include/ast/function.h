#ifndef HAARD_AST_FUNCTION_H
#define HAARD_AST_FUNCTION_H

#include <vector>
#include <string>
#include "token/token.h"
#include "ast/variable.h"
#include "ast/type.h"
#include "ast/type_list.h"
#include "ast/function_type.h"
#include "ast/compound_statement.h"
#include "ast/annotation.h"
#include "ast/compound_type_descriptor.h"

namespace haard {
    class Scope;
    class Class;
    class Module;
    class CompoundTypeDescriptor;

    class Function {
    public:
        Function();
        ~Function();

    public:
        int get_line();
        int get_column();
        std::string get_name();
        std::string get_type_signature();
        Variable* get_parameter(int idx);
        Variable* get_variable(int idx);
        Type* get_return_type();
        CompoundStatement* get_statements();
        Scope* get_scope();
        std::string get_qualified_name();
        std::string get_original();

        void set_line(int line);
        void set_column(int column);
        void set_name(std::string name);
        void set_from_token(Token& token);
        void set_return_type(Type* type);
        void set_statements(CompoundStatement* statements);
        void set_virtual(bool v);
        void set_template(bool v);

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
        bool is_constructor();
        bool is_destructor();
        bool is_virtual();

        Function* clone();
        bool same_signature(Function* other);

        void set_module(Module* module);
        Module* get_module();

        void set_overloaded_index(int idx);
        int get_overloaded_index();

        void set_template_header(TypeList* header);
        TypeList* get_template_header();

        std::vector<Annotation*> get_annotations() const;
        void set_annotations(const std::vector<Annotation*> &value);

        Function* get_parent_method() const;
        void set_parent_method(Function* value);

        void set_constructor(bool value);
        void set_destructor(bool value);

        std::string get_path();

        int get_begin() const;
        void set_begin(int value);

        int get_end() const;
        void set_end(int value);

        CompoundTypeDescriptor* get_compound() const;
        void set_compound(CompoundTypeDescriptor* value);

    private:
        int uid;
        int line;
        int column;
        int overloaded_index;
        int begin;
        int end;
        bool method_flag;
        bool constructor_flag;
        bool destructor_flag;
        bool virtual_flag;
        bool template_flag;
        std::string name;
        Type* return_type;
        FunctionType* self_type;
        TypeList* template_header;
        CompoundStatement* statements;
        Function* parent_method;
        Scope* scope;
        Module* module;
        CompoundTypeDescriptor* compound;
        std::vector<Variable*> parameters;
        std::vector<Variable*> variables;
        std::vector<Annotation*> annotations;
        std::vector<Function*> tfunctions;
    };
}

#endif
