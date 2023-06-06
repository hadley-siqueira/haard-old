#ifndef HAARD_AST_LIST_H
#define HAARD_AST_LIST_H

#include "expression.h"
#include "expression_list.h"

namespace haard {
    class List : public Expression {
    public:
        List();
        ~List();

    public:
        void add_expression(Expression* expr);
        int expressions_count();
        Expression* get_expression(int idx);

    private:
        ExpressionList* expressions;
    };
}

#endif
