#ifndef HAARD_AST_DIVISION_H
#define HAARD_AST_DIVISION_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Division : public BinaryOperator {
    public:
        Division(Expression* left=nullptr, Expression* right=nullptr);
        Division(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
