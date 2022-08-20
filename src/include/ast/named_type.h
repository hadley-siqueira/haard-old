#ifndef HAARD_AST_NAMED_TYPE_H
#define HAARD_AST_NAMED_TYPE_H

#include "ast/type.h"
#include "ast/template_header.h"
#include "ast/identifier.h"
#include "scope/symbol.h"

namespace haard {
    class NamedType : public Type {
        public:
            NamedType();
            ~NamedType();

        public:
            const char* get_alias();
            const char* get_name();
            Symbol* get_symbol();

            void set_alias(const char* id);
            void set_name(const char* id);
            void set_symbol(Symbol* symbol);

            Symbol* has_field(const char* name);
            bool equal(Type* type);

            std::string to_cpp();
            std::string to_str();

            Type* clone();

            void set_template_header(TemplateHeader* header);
            TemplateHeader* get_template_header();

        private:
            const char* alias;
            const char* name;
            TemplateHeader* template_header;
            Symbol* symbol;
    };
}

#endif
