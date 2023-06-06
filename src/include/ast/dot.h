#ifndef HAARD_AST_DOT_H
#define HAARD_AST_DOT_H

#include "token/token.h"
#include "binary_operator.h"

namespace haard {
    class Dot : public BinaryOperator {
    public:
        Dot(Expression* object=nullptr, Expression* field=nullptr);
        Dot(Token& token, Expression* object=nullptr, Expression* field=nullptr);

    public:
        Expression* get_object() const;
        void set_object(Expression* value);

        Expression* get_field() const;
        void set_field(Expression* value);
    };
}

#endif
