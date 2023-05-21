#ifndef HAARD_AST_PLUS_H
#define HAARD_AST_PLUS_H

#include "token/token.h"
#include "ast/expression.h"

namespace haard {
    class Plus : public Expression {
    public:
        Plus(Expression* left=nullptr, Expression* right=nullptr);
        Plus(Token& token, Expression* left=nullptr, Expression* right=nullptr);
        virtual ~Plus();

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
