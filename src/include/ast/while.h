#ifndef HAARD_AST_WHILE_STATEMENT_H
#define HAARD_AST_WHILE_STATEMENT_H

#include "ast/statement.h"
#include "ast/compound_statement.h"
#include "ast/expression.h"

namespace haard {
    class While : public Statement {
    public:
        While();
        ~While();

    public:
        Expression* get_condition();
        CompoundStatement* get_statements();

        void set_condition(Expression* expression);
        void set_statements(CompoundStatement* statements);

    private:
        Expression* condition;
        CompoundStatement* statements;
    };
}

#endif
