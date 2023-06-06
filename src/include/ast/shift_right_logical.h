#ifndef HAARD_AST_SHIFT_RIGHT_LOGICAL_H
#define HAARD_AST_SHIFT_RIGHT_LOGICAL_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class ShiftRightLogical : public BinaryOperator {
    public:
        ShiftRightLogical(Expression* left=nullptr, Expression* right=nullptr);
        ShiftRightLogical(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
