#ifndef HAARD_AST_DIVISION_ASSIGNMENT_H
#define HAARD_AST_DIVISION_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class DivisionAssignment : public BinaryOperator {
    public:
        DivisionAssignment(Expression* left=nullptr, Expression* right=nullptr);
        DivisionAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
