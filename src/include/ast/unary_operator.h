#ifndef HAARD_AST_UNARY_OPERATOR_H
#define HAARD_AST_UNARY_OPERATOR_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class UnaryOperator : public Expression {
    public:
        UnaryOperator(Expression* expression=nullptr);
        UnaryOperator(Token& token, Expression* expression=nullptr);
        ~UnaryOperator();

    public:
        Expression* get_expression() const;
        void set_expression(Expression* expression);

    private:
        Expression* expression;
    };
}

#endif
