#ifndef HAARD_AST_TIMES_ASSIGNMENT_H
#define HAARD_AST_TIMES_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class TimesAssignment : public BinaryOperator {
    public:
        TimesAssignment(Expression* left=nullptr, Expression* right=nullptr);
        TimesAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
