#ifndef HAARD_AST_MINUS_H
#define HAARD_AST_MINUS_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class Minus : public BinaryOperator {
    public:
        Minus(Expression* left=nullptr, Expression* right=nullptr);
        Minus(Token& token, Expression* left=nullptr, Expression* right=nullptr);
        ~Minus();

    public:
        Expression* get_left() const;
        void set_left(Expression* value);

        Expression* get_right() const;
        void set_right(Expression* value);

    private:
        int line;
        int column;
        Expression* left;
        Expression* right;
    };
}

#endif
