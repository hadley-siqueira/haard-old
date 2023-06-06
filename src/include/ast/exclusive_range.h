#ifndef HAARD_AST_EXCLUSIVE_RANGE_H
#define HAARD_AST_EXCLUSIVE_RANGE_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class ExclusiveRange : public BinaryOperator {
    public:
        ExclusiveRange(Expression* left=nullptr, Expression* right=nullptr);
        ExclusiveRange(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
