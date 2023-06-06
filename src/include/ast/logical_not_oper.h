#ifndef HAARD_AST_LOGICAL_NOT_OPER_H
#define HAARD_AST_LOGICAL_NOT_OPER_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class LogicalNotOper : public UnaryOperator {
    public:
        LogicalNotOper(Expression* expression=nullptr);
        LogicalNotOper(Token& token, Expression* expression=nullptr);
    };
}

#endif
