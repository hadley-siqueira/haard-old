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
            std::string get_name();
            std::string get_cpp_name();
            int get_line();
            int get_column();
            int get_uid();
            Function* get_method(int idx);
            Variable* get_variable(int idx);
            Scope* get_scope();
            NamedType* get_self_type();
            int get_size_in_bytes();
            std::string get_qualified_name();

            void set_from_token(Token& token);
            void set_name(std::string name);
            void set_line(int line);
            void set_column(int column);
            void set_uid(int uid);
            void set_self_type(NamedType* type);

            void add_method(Function* method);
            void add_variable(Variable* var);
            void calculate_variables_offset();

            int methods_count();
            int variables_count();

            int constructors_count();
            Function* get_constructor(int idx);

            Source* get_source();
            void set_source(Source* source);

            Type* get_super_class();
            void set_super_class(Type* type);
            bool has_super_class();
            Class* get_super_class_descriptor();

            void set_template_header(TypeList* header);
            TypeList* get_template_header();

            Function* get_destructor() const;
            void set_destructor(Function* value);

            std::vector<std::string> get_annotations() const;
            void set_annotations(const std::vector<std::string>& value);

            int get_alignment() const;
            void set_alignment(int value);

            void set_virtual(bool flag);
            bool is_virtual();

            int get_remaining_pad() const;
            void set_remaining_pad(int value);

            bool is_template();
            void set_template(bool value);

    private:
            int line;
            int column;
            int uid;
            int size_in_bytes;
            int alignment;
            int remaining_pad;
            bool is_virtual_flag;
            bool template_flag;
            std::string name;
            Type* super_class;
            TypeList* template_header;
            Function* destructor;
            Scope* scope;
            NamedType* self_type;
            Source* source; // source where this class is defined
            std::vector<Function*> methods;
            std::vector<Function*> constructors;
            std::vector<Variable*> variables;
            std::vector<std::string> annotations;
    };
}

#endif
