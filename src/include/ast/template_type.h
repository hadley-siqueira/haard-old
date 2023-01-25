#ifndef HAARD_AST_TEMPLATE_TYPE_H
#define HAARD_AST_TEMPLATE_TYPE_H

#include <string>
#include "ast/type.h"
#include "token/token.h"

namespace haard {
    class TemplateType : public Type {
        public:
            TemplateType();
            TemplateType(Token& token);

        public:
            std::string get_name();
            void set_name(std::string name);

            std::string to_str();
            std::string to_cpp();
            std::string get_qualified_name();
            bool equal(Type* type);
            int get_size_in_bytes();
            int get_alignment();

            Type* get_bind_type();
            void set_bind_type(Type* type);
            bool is_binded();

            Type* clone();

        private:
            std::string name;
            Type* bind_type;
    };
}

#endif
