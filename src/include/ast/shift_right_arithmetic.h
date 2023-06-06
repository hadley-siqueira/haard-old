#ifndef HAARD_AST_SHIFT_RIGHT_ARITHMETIC_H
#define HAARD_AST_SHIFT_RIGHT_ARITHMETIC_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class ShiftRightArithmetic : public BinaryOperator {
    public:
        ShiftRightArithmetic(Expression* left=nullptr, Expression* right=nullptr);
        ShiftRightArithmetic(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
