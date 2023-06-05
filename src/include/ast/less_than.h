#ifndef HAARD_AST_LESS_THAN_H
#define HAARD_AST_LESS_THAN_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class LessThan : public BinaryOperator {
    public:
        LessThan(Expression* left=nullptr, Expression* right=nullptr);
        LessThan(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
