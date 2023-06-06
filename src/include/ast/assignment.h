#ifndef HAARD_AST_ASSIGNMENT_H
#define HAARD_AST_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Assignment : public BinaryOperator {
    public:
        Assignment(Expression* left=nullptr, Expression* right=nullptr);
        Assignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
