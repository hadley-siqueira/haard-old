#ifndef HAARD_AST_DELETE_ARRAY_H
#define HAARD_AST_DELETE_ARRAY_H

#include "unary_operator.h"

namespace haard {
    class DeleteArray : public UnaryOperator {
    public:
        DeleteArray(Token& token, Expression* expression=nullptr);
    };
}

#endif
