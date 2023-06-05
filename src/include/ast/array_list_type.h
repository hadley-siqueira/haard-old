#ifndef HAARD_AST_ARRAY_LIST_TYPE_H
#define HAARD_AST_ARRAY_LIST_TYPE_H

#include "token/token.h"
#include "ast/type.h"
#include "ast/expression.h"

namespace haard {
    class ArrayListType : public Type {
        public:
            ArrayListType();
            ArrayListType(int kind, Token& token, Type* subtype=nullptr, Expression* expr=nullptr);
            ~ArrayListType();

        public:
            Expression* get_expression();
            Type* get_subtype();

            /* virtual */
            bool equal(Type* type);
            bool is_primitive();
            Type* promote(Type* other);
            int rank();
            int get_size_in_bytes();
            int get_alignment();
            
        private:
            Expression* expression;
            Type* subtype;
    };
}

#endif
