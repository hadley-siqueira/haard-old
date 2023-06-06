#ifndef HAARD_AST_ADDRESS_OF_H
#define HAARD_AST_ADDRESS_OF_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class AddressOf : public UnaryOperator {
    public:
        AddressOf(Expression* expression=nullptr);
        AddressOf(Token& token, Expression* expression=nullptr);
    };
}

#endif
