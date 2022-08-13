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
            Type* clone();
            
        private:
            Expression* expression;
            Type* subtype;
    };
}

#endif
