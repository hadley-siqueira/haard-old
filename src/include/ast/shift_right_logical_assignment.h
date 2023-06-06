#ifndef HAARD_AST_SHIFT_RIGHT_LOGICAL_ASSIGNMENT_H
#define HAARD_AST_SHIFT_RIGHT_LOGICAL_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class ShiftRightLogicalAssignment : public BinaryOperator {
    public:
        ShiftRightLogicalAssignment(Expression* left=nullptr, Expression* right=nullptr);
        ShiftRightLogicalAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
