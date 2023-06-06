#ifndef HAARD_AST_UNARY_PLUS_H
#define HAARD_AST_UNARY_PLUS_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class UnaryPlus : public UnaryOperator {
    public:
        UnaryPlus(Expression* expression=nullptr);
        UnaryPlus(Token& token, Expression* expression=nullptr);
    };
}

#endif
