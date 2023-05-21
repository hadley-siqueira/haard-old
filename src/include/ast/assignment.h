#ifndef HAARD_AST_ASSIGNMENT_H
#define HAARD_AST_ASSIGNMENT_H

#include "token/token.h"
#include "ast/expression.h"

namespace haard {
    class Assignment : public Expression {
    public:
        Assignment(Expression* left=nullptr, Expression* right=nullptr);
        virtual ~Assignment();

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
