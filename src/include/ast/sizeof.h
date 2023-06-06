#ifndef HAARD_AST_SIZEOF_H
#define HAARD_AST_SIZEOF_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class Sizeof : public UnaryOperator {
    public:
        Sizeof(Expression* expression=nullptr);
        Sizeof(Token& token, Expression* expression=nullptr);
    };
}

#endif
