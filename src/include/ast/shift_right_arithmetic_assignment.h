#ifndef HAARD_AST_SHIFT_RIGHT_ARITHMETIC_ASSIGNMENT_H
#define HAARD_AST_SHIFT_RIGHT_ARITHMETIC_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class ShiftRightArithmeticAssignment : public BinaryOperator {
    public:
        ShiftRightArithmeticAssignment(Expression* left=nullptr, Expression* right=nullptr);
        ShiftRightArithmeticAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
