#ifndef HAARD_AST_BITWISE_XOR_ASSIGNMENT_H
#define HAARD_AST_BITWISE_XOR_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class BitwiseXorAssignment : public BinaryOperator {
    public:
        BitwiseXorAssignment(Expression* left=nullptr, Expression* right=nullptr);
        BitwiseXorAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
