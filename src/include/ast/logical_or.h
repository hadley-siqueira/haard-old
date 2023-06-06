#ifndef HAARD_AST_LOGICAL_OR_H
#define HAARD_AST_LOGICAL_OR_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class LogicalOr : public BinaryOperator {
    public:
        LogicalOr(Expression* left=nullptr, Expression* right=nullptr);
        LogicalOr(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
