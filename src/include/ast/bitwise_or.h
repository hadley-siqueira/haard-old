#ifndef HAARD_AST_BITWISE_OR_H
#define HAARD_AST_BITWISE_OR_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class BitwiseOr : public BinaryOperator {
    public:
        BitwiseOr(Expression* left=nullptr, Expression* right=nullptr);
        BitwiseOr(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
