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
    class Statement;
    class Module;

    class Class : public CompoundTypeDescriptor {
    public:
        Class();
        ~Class();

    public:
        int get_size_in_bytes();
        std::string get_qualified_name();

        void set_from_token(Token& token);
        void calculate_variables_offset();

        Function* get_constructor(int idx);

        void set_virtual(bool flag);
        bool is_virtual();

        int get_remaining_pad() const;
        void set_remaining_pad(int value);

    private:
        int remaining_pad;
        int uid;
        bool is_virtual_flag;
    };
}

#endif
