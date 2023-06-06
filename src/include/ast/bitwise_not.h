#ifndef HAARD_AST_BITWISE_NOT_H
#define HAARD_AST_BITWISE_NOT_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class BitwiseNot : public UnaryOperator {
    public:
        BitwiseNot(Expression* expression=nullptr);
        BitwiseNot(Token& token, Expression* expression=nullptr);
    };
}

#endif
