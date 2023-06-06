#ifndef HAARD_AST_GREATER_THAN_OR_EQUAL_H
#define HAARD_AST_GREATER_THAN_OR_EQUAL_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class GreaterThanOrEqual : public BinaryOperator {
    public:
        GreaterThanOrEqual(Expression* left=nullptr, Expression* right=nullptr);
        GreaterThanOrEqual(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
