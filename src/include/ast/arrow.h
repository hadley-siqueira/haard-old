#ifndef HAARD_AST_ARROW_H
#define HAARD_AST_ARROW_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Arrow : public BinaryOperator {
    public:
        Arrow(Expression* object=nullptr, Expression* field=nullptr);
        Arrow(Token& token, Expression* object=nullptr, Expression* field=nullptr);

    public:
        Expression* get_object() const;
        void set_object(Expression* value);

        Expression* get_field() const;
        void set_field(Expression* value);
    };
}

#endif
