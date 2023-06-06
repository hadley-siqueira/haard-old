#ifndef HAARD_AST_PLUS_H
#define HAARD_AST_PLUS_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Plus : public BinaryOperator {
    public:
        Plus(Expression* left=nullptr, Expression* right=nullptr);
        Plus(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
