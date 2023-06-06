#ifndef HAARD_AST_LOGICAL_AND_OPER_H
#define HAARD_AST_LOGICAL_AND_OPER_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class LogicalAndOper : public BinaryOperator {
    public:
        LogicalAndOper(Expression* left=nullptr, Expression* right=nullptr);
        LogicalAndOper(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
