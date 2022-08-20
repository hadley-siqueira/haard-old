#ifndef HAARD_AST_FUNCTION_TYPE_H
#define HAARD_AST_FUNCTION_TYPE_H

#include <vector>
#include "ast/type.h"
#include "ast/template_header.h"

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
            TemplateHeader* get_template_header();

            // override
            bool equal(Type* type);
            Type* clone();
            std::string to_str();
            
        private:
            std::vector<Type*> ptypes;
            Type* rtype;
            TemplateHeader* template_header;
            
    };
}

#endif
