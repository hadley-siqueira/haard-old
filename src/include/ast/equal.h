#ifndef HAARD_AST_EQUAL_H
#define HAARD_AST_EQUAL_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Equal : public BinaryOperator {
    public:
        Equal(Expression* left=nullptr, Expression* right=nullptr);
        Equal(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
