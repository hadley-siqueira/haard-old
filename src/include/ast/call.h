#ifndef HAARD_AST_CALL_H
#define HAARD_AST_CALL_H

#include "token/token.h"
#include "expression.h"
#include "expression_list.h"

namespace haard {
    class Call : public Expression {
    public:
        int get_line() const;
        void set_line(int value);

        int get_column() const;
        void set_column(int value);

        Expression* get_object() const;
        void set_object(Expression* value);

        ExpressionList* get_arguments() const;
        void set_arguments(ExpressionList* value);

    private:
        int line;
        int column;
        Expression* object;
        ExpressionList* arguments;
    };
}

#endif
