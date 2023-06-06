#ifndef HAARD_AST_MINUS_H
#define HAARD_AST_MINUS_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Minus : public BinaryOperator {
    public:
        Minus(Expression* left=nullptr, Expression* right=nullptr);
        Minus(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
