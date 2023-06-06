#ifndef HAARD_AST_BITWISE_AND_ASSIGNMENT_H
#define HAARD_AST_BITWISE_AND_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class BitwiseAndAssignment : public BinaryOperator {
    public:
        BitwiseAndAssignment(Expression* left=nullptr, Expression* right=nullptr);
        BitwiseAndAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
