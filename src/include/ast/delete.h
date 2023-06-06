#ifndef HAARD_AST_DELETE_H
#define HAARD_AST_DELETE_H

#include "unary_operator.h"

namespace haard {
    class Delete : public UnaryOperator {
    public:
        Delete(Token& token, Expression* expression=nullptr);
    };
}

#endif
