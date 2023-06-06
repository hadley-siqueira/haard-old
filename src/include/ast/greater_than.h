#ifndef HAARD_AST_GREATER_THAN_H
#define HAARD_AST_GREATER_THAN_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class GreaterThan : public BinaryOperator {
    public:
        GreaterThan(Expression* left=nullptr, Expression* right=nullptr);
        GreaterThan(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
