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
            ~NamedType();

        public:
            const char* get_alias();
            const char* get_name();
            Symbol* get_symbol();

            void set_alias(const char* id);
            void set_name(const char* id);
            void set_symbol(Symbol* symbol);
            int get_size_in_bytes();
            int get_alignment();
            Scope* get_scope();
            bool is_class();
            std::string get_qualified_name();

            Symbol* has_field(const char* name);
            bool equal(Type* type);

            std::string to_cpp();
            std::string to_str();

            Type* clone();

            void set_template_header(TypeList* header);
            TypeList* get_template_header();

        private:
            const char* alias;
            const char* name;
            TypeList* template_header;
            Symbol* symbol;
    };
}

#endif
