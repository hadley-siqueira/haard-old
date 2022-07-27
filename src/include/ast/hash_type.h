#ifndef HAARD_AST_HASH_TYPE
#define HAARD_AST_HASH_TYPE

#include "token/token.h"
#include "ast/type.h"

namespace haard {
    class HashType : public Type {
        public:
            HashType(Token& token, Type* key, Type* value);
            ~HashType();

        public:
            Type* get_key_type();
            Type* get_value_type();

        private:
            Type* key;
            Type* value;
    };
}

#endif
