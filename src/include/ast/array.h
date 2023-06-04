#ifndef HAARD_AST_ARRAY_H
#define HAARD_AST_ARRAY_H

#include "expression.h"
#include "expression_list.h"

namespace haard {
    class Array : public Expression {
    public:
        Array();
        ~Array();

    public:
        void add_expression(Expression* expr);
        int expressions_count();
        Expression* get_expression(int idx);
        std::string to_str();

    private:
        ExpressionList* expressions;
    };
}

#endif
