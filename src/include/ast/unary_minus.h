#ifndef HAARD_AST_UNARY_MINUS_H
#define HAARD_AST_UNARY_MINUS_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class UnaryMinus : public UnaryOperator {
    public:
        UnaryMinus(Expression* expression=nullptr);
        UnaryMinus(Token& token, Expression* expression=nullptr);
    };
}

#endif
