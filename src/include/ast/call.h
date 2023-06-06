#ifndef HAARD_AST_CALL_H
#define HAARD_AST_CALL_H

#include "token/token.h"
#include "expression.h"
#include "expression_list.h"

namespace haard {
    class Call : public Expression {
    public:
        Call(Expression* object=nullptr, ExpressionList* arguments=nullptr);
        Call(Token& token, Expression* object=nullptr, ExpressionList* arguments=nullptr);
        ~Call();

    public:
        Expression* get_object() const;
        void set_object(Expression* value);

        ExpressionList* get_arguments() const;
        void set_arguments(ExpressionList* value);

    private:
        Expression* object;
        ExpressionList* arguments;
    };
}

#endif
