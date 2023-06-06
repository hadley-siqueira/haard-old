#ifndef HAARD_AST_INTEGER_DIVISION_H
#define HAARD_AST_INTEGER_DIVISION_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class IntegerDivision : public BinaryOperator {
    public:
        IntegerDivision(Expression* left=nullptr, Expression* right=nullptr);
        IntegerDivision(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
