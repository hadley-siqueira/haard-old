#ifndef HAARD_AST_ADDRESS_OF_H
#define HAARD_AST_ADDRESS_OF_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class AddressOf : public Expression {
    public:
        AddressOf(Expression* expression=nullptr);
        AddressOf(Token& token, Expression* expression=nullptr);
        ~AddressOf();

    public:
        Expression* get_expression() const;
        void set_expression(Expression* expression);

    private:
        Expression* expression;
    };
}

#endif
