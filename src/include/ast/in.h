#ifndef HAARD_AST_IN_H
#define HAARD_AST_IN_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class In : public BinaryOperator {
    public:
        In(Expression* left=nullptr, Expression* right=nullptr);
        In(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
