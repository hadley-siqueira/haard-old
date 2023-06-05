#ifndef HAARD_AST_BINARY_OPERATOR_H
#define HAARD_AST_BINARY_OPERATOR_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class BinaryOperator : public Expression {
    public:
        BinaryOperator();
        ~BinaryOperator();

    public:
        Expression* get_left() const;
        void set_left(Expression* value);

        Expression* get_right() const;
        void set_right(Expression* value);

    private:
        Expression* left;
        Expression* right;
    };
}

#endif
