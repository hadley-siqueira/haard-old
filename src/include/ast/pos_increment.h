#ifndef HAARD_AST_POS_INCREMENT_H
#define HAARD_AST_POS_INCREMENT_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class PosIncrement : public UnaryOperator {
    public:
        PosIncrement(Expression* expression=nullptr);
        PosIncrement(Token& token, Expression* expression=nullptr);
    };
}

#endif
