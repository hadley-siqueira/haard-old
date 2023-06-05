#ifndef HAARD_AST_STRING_BUILDER_H
#define HAARD_AST_STRING_BUILDER_H

#include <vector>
#include "token/token.h"
#include "ast/expression.h"
#include "ast/variable.h"

namespace haard {
    class StringBuilder : public Expression {
    public:
        StringBuilder();
        ~StringBuilder();

    public:
        void add_expression(Expression* expr);
        int expressions_count();
        Expression* get_expression(int i);

        void add_call(Expression* expr);
        int calls_count();
        Expression* get_call(int i);

        Variable* get_variable() const;
        void set_variable(Variable *value);

    private:
        std::vector<Expression*> expressions;
        std::vector<Expression*> calls;
        Variable* variable;
    };
}

#endif
