#ifndef HAARD_AST_WHILE_STATEMENT_H
#define HAARD_AST_WHILE_STATEMENT_H

#include "ast/statement.h"
#include "ast/compound_statement.h"
#include "ast/expression.h"
#include "scope/scope.h"

namespace haard {
    class WhileStatement : public Statement {
    public:
        WhileStatement();
        ~WhileStatement();

    public:
        Expression* get_condition();
        CompoundStatement* get_statements();

        void set_condition(Expression* expression);
        void set_statements(CompoundStatement* statements);

        Scope* get_scope();

    private:
        Expression* condition;
        CompoundStatement* statements;
        Scope* scope;
    };
}

#endif
