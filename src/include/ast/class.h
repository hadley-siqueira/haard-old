#ifndef HAARD_AST_CLASS_H
#define HAARD_AST_CLASS_H

#include <vector>
#include "token/token.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/type.h"
#include "ast/named_type.h"
#include "ast/annotation.h"
#include "ast/compound_type_descriptor.h"

namespace haard {
    class Scope;
    class Module;

    class Class : public CompoundTypeDescriptor {
    public:
        Class();
        ~Class();

    public:
        std::string get_cpp_name();
        int get_uid();
        Variable* get_variable(int idx);
        NamedType* get_self_type();
        int get_size_in_bytes();
        std::string get_qualified_name();
        std::string get_original();

        void set_from_token(Token& token);
        void set_uid(int uid);
        void set_self_type(NamedType* type);

        void add_variable(Variable* var);
        void calculate_variables_offset();

        int variables_count();

        Function* get_constructor(int idx);

        void set_virtual(bool flag);
        bool is_virtual();

        int get_remaining_pad() const;
        void set_remaining_pad(int value);

        bool is_template();
        void set_template(bool value);

        std::string get_path();

    private:
        int uid;
        int remaining_pad;
        bool is_virtual_flag;
        std::vector<Variable*> variables;
    };
}

#endif
