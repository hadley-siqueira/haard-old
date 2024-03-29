#ifndef HAARD_AST_NAMED_TYPE_H
#define HAARD_AST_NAMED_TYPE_H

#include "ast/type.h"
#include "ast/identifier.h"
#include "scope/symbol.h"
#include "ast/type_list.h"

namespace haard {
    class NamedType : public Type {
        public:
            NamedType();
            NamedType(Token& token);
            ~NamedType();

        public:
            std::string get_alias();
            std::string get_name();
            SymbolDescriptor* get_symbol_descriptor();

            void set_alias(std::string id);
            void set_name(std::string id);
            void set_symbol_descriptor(SymbolDescriptor* value);
            int get_size_in_bytes();
            int get_alignment();
            Scope* get_scope();
            bool is_class();
            std::string get_qualified_name();

            bool equal(Type* type);
            bool is_integer_scalar();

            std::string to_str();

            void set_template_header(TemplateHeader *header);
            TemplateHeader* get_template_header();

            Type* get_bind_type();
            void set_bind_type(Type* type);
            bool is_binded();

        private:
            Identifier* id;
    };
}

#endif
