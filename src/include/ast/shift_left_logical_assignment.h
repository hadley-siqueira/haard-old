#ifndef HAARD_AST_SHIFT_LEFT_LOGICAL_ASSIGNMENT_H
#define HAARD_AST_SHIFT_LEFT_LOGICAL_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class ShiftLeftLogicalAssignment : public BinaryOperator {
    public:
        ShiftLeftLogicalAssignment(Expression* left=nullptr, Expression* right=nullptr);
        ShiftLeftLogicalAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
