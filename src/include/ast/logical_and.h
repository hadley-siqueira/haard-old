#ifndef HAARD_AST_LOGICAL_AND_H
#define HAARD_AST_LOGICAL_AND_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class LogicalAnd : public BinaryOperator {
    public:
        LogicalAnd(Expression* left=nullptr, Expression* right=nullptr);
        LogicalAnd(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
