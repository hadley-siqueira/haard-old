#ifndef HAARD_AST_TIMES_H
#define HAARD_AST_TIMES_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Times : public BinaryOperator {
    public:
        Times(Expression* left=nullptr, Expression* right=nullptr);
        Times(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
