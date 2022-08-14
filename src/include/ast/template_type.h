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
            const char* get_name();
            void set_name(const char* name);

            std::string to_str();
            std::string to_cpp();

            Type* get_bind_type();
            void set_bind_type(Type* type);
            bool is_binded();

            Type* clone();

        private:
            const char* name;
            Type* bind_type;
    };
}

#endif
