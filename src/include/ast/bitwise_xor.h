#ifndef HAARD_AST_BITWISE_XOR_H
#define HAARD_AST_BITWISE_XOR_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class BitwiseXor : public BinaryOperator {
    public:
        BitwiseXor(Expression* left=nullptr, Expression* right=nullptr);
        BitwiseXor(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
