#ifndef HAARD_AST_MODULO_ASSIGNMENT_H
#define HAARD_AST_MODULO_ASSIGNMENT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class ModuloAssignment : public BinaryOperator {
    public:
        ModuloAssignment(Expression* left=nullptr, Expression* right=nullptr);
        ModuloAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
    };
}

#endif
