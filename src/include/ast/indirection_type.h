#ifndef HAARD_AST_INDIRECTION_TYPE_H
#define HAARD_AST_INDIRECTION_TYPE_H

#include "token/token.h"
#include "ast/type.h"

namespace haard {
    class IndirectionType : public Type {
        public:
            IndirectionType(int kind, Token& token, Type* subtype);
            IndirectionType(int kind, Type* subtype);
            ~IndirectionType();

        public:
            Type* get_subtype();
            bool equal(Type* type);

        private:
            Type* subtype;
    };
}

#endif
