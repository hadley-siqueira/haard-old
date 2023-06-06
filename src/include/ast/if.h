#ifndef HAARD_AST_IF_H
#define HAARD_AST_IF_H

#include "ast/statement.h"
#include "ast/expression.h"
#include "scope/scope.h"

namespace haard {
    class If : public Statement {
    public:
        If();
        ~If();

    public:
        Expression* get_condition() const;
        void set_condition(Expression* value);

        Statement* get_true_statements() const;
        void set_true_statements(Statement* value);

        Statement* get_false_statements() const;
        void set_false_statements(Statement* value);

    private:
        Expression* condition;
        Statement* true_statements;
        Statement* false_statements;
    };
}

#endif
