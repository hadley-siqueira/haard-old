#ifndef HAARD_AST_FUNCTION_TYPE_H
#define HAARD_AST_FUNCTION_TYPE_H

#include <vector>
#include "ast/type.h"
#include "ast/type_list.h"

namespace haard {
    class FunctionType : public Type {
    public:
        FunctionType();
        ~FunctionType();

    public:
        void add_param_type(Type* type);
        void set_return_type(Type* type);

        int params_count();
        Type* get_param_type(int idx);
        Type* get_return_type();
        void add_template(Type* type);
        int templates_count();
        Type* get_template(int idx);
        TypeList* get_template_header();
        bool check_arguments_type(TypeList* args);
        bool check_arguments_type_with_conversion(TypeList* args);

        // override
        bool equal(Type* type);
        Type* clone();
        std::string to_str();
        std::string get_qualified_name();

    private:
        std::vector<Type*> ptypes;
        Type* rtype;
        TypeList* template_header;
    };
}

#endif
