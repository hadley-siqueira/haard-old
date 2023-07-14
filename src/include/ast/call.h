#ifndef HAARD_AST_CALL_H
#define HAARD_AST_CALL_H

#include "token/token.h"
#include "expression.h"
#include "expression_list.h"

namespace haard {
    class Function;

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

        Function* get_function() const;
        void set_function(Function *value);

    private:
        Expression* object;
        Function* function;
        ExpressionList* arguments;
    };
}

#endif
