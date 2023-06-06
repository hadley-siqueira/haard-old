#ifndef HAARD_AST_NOT_EQUAL_H
#define HAARD_AST_NOT_EQUAL_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class NotEqual : public BinaryOperator {
    public:
        NotEqual(Expression* left=nullptr, Expression* right=nullptr);
        NotEqual(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
