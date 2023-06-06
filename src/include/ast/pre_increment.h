#ifndef HAARD_AST_PRE_INCREMENT_H
#define HAARD_AST_PRE_INCREMENT_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class PreIncrement : public UnaryOperator {
    public:
        PreIncrement(Expression* expression=nullptr);
        PreIncrement(Token& token, Expression* expression=nullptr);
    };
}

#endif
