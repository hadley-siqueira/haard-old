#ifndef HAARD_AST_NOT_IN_H
#define HAARD_AST_NOT_IN_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class NotIn : public BinaryOperator {
    public:
        NotIn(Expression* left=nullptr, Expression* right=nullptr);
        NotIn(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
