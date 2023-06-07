#ifndef HAARD_AST_HASH_H
#define HAARD_AST_HASH_H

#include "expression.h"
#include "expression_list.h"

namespace haard {
    class Hash : public Expression {
    public:
        Hash();
        ~Hash();

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
