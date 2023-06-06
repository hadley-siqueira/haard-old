#ifndef HAARD_AST_INTEGER_DIVISION_ASSIGNMENT_H
#define HAARD_AST_INTEGER_DIVISION_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class IntegerDivisionAssignment : public BinaryOperator {
    public:
        IntegerDivisionAssignment(Expression* left=nullptr, Expression* right=nullptr);
        IntegerDivisionAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
