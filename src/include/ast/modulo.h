#ifndef HAARD_AST_MODULO_H
#define HAARD_AST_MODULO_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Modulo : public BinaryOperator {
    public:
        Modulo(Expression* left=nullptr, Expression* right=nullptr);
        Modulo(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
