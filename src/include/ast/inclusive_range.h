#ifndef HAARD_AST_INCLUSIVE_RANGE_H
#define HAARD_AST_INCLUSIVE_RANGE_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class InclusiveRange : public BinaryOperator {
    public:
        InclusiveRange(Expression* left=nullptr, Expression* right=nullptr);
        InclusiveRange(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
