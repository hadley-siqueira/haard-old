#ifndef HAARD_AST_INDEX_H
#define HAARD_AST_INDEX_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Index : public BinaryOperator {
    public:
        Index(Expression* object=nullptr, Expression* index=nullptr);
        Index(Token& token, Expression* object=nullptr, Expression* index=nullptr);

    public:
        Expression* get_object() const;
        void set_object(Expression* value);

        Expression* get_index() const;
        void set_index(Expression* value);
    };
}

#endif
