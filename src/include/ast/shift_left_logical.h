#ifndef HAARD_AST_SHIFT_LEFT_LOGICAL_H
#define HAARD_AST_SHIFT_LEFT_LOGICAL_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class ShiftLeftLogical : public BinaryOperator {
    public:
        ShiftLeftLogical(Expression* left=nullptr, Expression* right=nullptr);
        ShiftLeftLogical(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
