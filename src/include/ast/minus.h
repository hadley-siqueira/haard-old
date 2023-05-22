#ifndef HAARD_AST_MINUS_H
#define HAARD_AST_MINUS_H

#include "token/token.h"
#include "ast/expression.h"

namespace haard {
    class Minus : public Expression {
    public:
        Minus(Expression* left=nullptr, Expression* right=nullptr);
        Minus(Token& token, Expression* left=nullptr, Expression* right=nullptr);
        virtual ~Minus();

    public:
        Expression* get_left() const;
        void set_left(Expression* value);

        Expression* get_right() const;
        void set_right(Expression* value);

        std::string to_str();

    private:
        Expression* left;
        Expression* right;
        int line;
        int column;
    };
}

#endif
