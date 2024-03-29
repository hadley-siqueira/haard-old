#ifndef HAARD_AST_TUPLE_H
#define HAARD_AST_TUPLE_H

#include "expression.h"
#include "expression_list.h"

namespace haard {
    class Tuple : public Expression {
    public:
        Tuple();
        ~Tuple();

    public:
        void add_expression(Expression* expr);
        int expressions_count();
        Expression* get_expression(int idx);

        ExpressionList* get_expressions() const;
        void set_expressions(ExpressionList* value);

    private:
        ExpressionList* expressions;
    };
}

#endif
