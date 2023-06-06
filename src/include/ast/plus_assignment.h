#ifndef HAARD_AST_PLUS_ASSIGNMENT_H
#define HAARD_AST_PLUS_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class PlusAssignment : public BinaryOperator {
    public:
        PlusAssignment(Expression* left=nullptr, Expression* right=nullptr);
        PlusAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
