#ifndef HAARD_AST_DEREFERENCE_H
#define HAARD_AST_DEREFERENCE_H

#include "token/token.h"
#include "unary_operator.h"

namespace haard {
    class Dereference : public UnaryOperator {
    public:
        Dereference(Expression* expression=nullptr);
        Dereference(Token& token, Expression* expression=nullptr);
    };
}

#endif
