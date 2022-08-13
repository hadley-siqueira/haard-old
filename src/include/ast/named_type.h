#ifndef HAARD_AST_NAMED_TYPE_H
#define HAARD_AST_NAMED_TYPE_H

#include "ast/type.h"
#include "ast/identifier.h"
#include "scope/symbol.h"

namespace haard {
    class NamedType : public Type {
        public:
            NamedType();
            ~NamedType();

        public:
            Identifier* get_alias();
            Identifier* get_name();
            Symbol* get_symbol();

            void set_alias(Identifier* id);
            void set_name(Identifier* id);
            void set_symbol(Symbol* symbol);

            Symbol* has_field(const char* name);

            std::string to_cpp();
            std::string to_str();

        private:
            Identifier* alias;
            Identifier* name;
            Symbol* symbol;
    };
}

#endif
