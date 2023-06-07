#ifndef HAARD_AST_HASH_PAIR_H
#define HAARD_AST_HASH_PAIR_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class HashPair : public BinaryOperator {
    public:
        HashPair(Expression* left=nullptr, Expression* right=nullptr);
        HashPair(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
