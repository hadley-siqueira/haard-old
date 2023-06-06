#ifndef HAARD_AST_SPECIAL_ASSIGNMENT_H
#define HAARD_AST_SPECIAL_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class SpecialAssignment : public BinaryOperator {
    public:
        SpecialAssignment(Expression* left=nullptr, Expression* right=nullptr);
        SpecialAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
