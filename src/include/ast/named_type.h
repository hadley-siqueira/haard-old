#ifndef HAARD_AST_NAMED_TYPE_H
#define HAARD_AST_NAMED_TYPE_H

#include "ast/type.h"
#include "ast/identifier.h"

namespace haard {
    class NamedType : public Type {
        public:
            NamedType();
            ~NamedType();

        public:
            Identifier* get_alias();
            Identifier* get_name();

            void set_alias(Identifier* id);
            void set_name(Identifier* id);

        private:
            Identifier* alias;
            Identifier* name;
    };
}

#endif
