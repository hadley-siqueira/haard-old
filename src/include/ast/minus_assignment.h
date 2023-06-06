#ifndef HAARD_AST_MINUS_ASSIGNMENT_H
#define HAARD_AST_MINUS_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class MinusAssignment : public BinaryOperator {
    public:
        MinusAssignment(Expression* left=nullptr, Expression* right=nullptr);
        MinusAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
