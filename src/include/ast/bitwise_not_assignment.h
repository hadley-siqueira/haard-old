#ifndef HAARD_AST_BITWISE_NOT_ASSIGNMENT_H
#define HAARD_AST_BITWISE_NOT_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class BitwiseNotAssignment : public BinaryOperator {
    public:
        BitwiseNotAssignment(Expression* left=nullptr, Expression* right=nullptr);
        BitwiseNotAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
