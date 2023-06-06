#ifndef HAARD_AST_LOGICAL_OR_OPER_H
#define HAARD_AST_LOGICAL_OR_OPER_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class LogicalOrOper : public BinaryOperator {
    public:
        LogicalOrOper(Expression* left=nullptr, Expression* right=nullptr);
        LogicalOrOper(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
