#ifndef HAARD_AST_BITWISE_OR_ASSIGNMENT_H
#define HAARD_AST_BITWISE_OR_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class BitwiseOrAssignment : public BinaryOperator {
    public:
        BitwiseOrAssignment(Expression* left=nullptr, Expression* right=nullptr);
        BitwiseOrAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
