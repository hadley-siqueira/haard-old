#ifndef HAARD_AST_POWER_H
#define HAARD_AST_POWER_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Power : public BinaryOperator {
    public:
        Power(Expression* left=nullptr, Expression* right=nullptr);
        Power(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
