#ifndef HAARD_AST_BRANCH_STATEMENT_H
#define HAARD_AST_BRANCH_STATEMENT_H

#include "ast/statement.h"
#include "ast/expression.h"

namespace haard {
    class BranchStatement : public Statement {
        public:
            BranchStatement(int kind);
            ~BranchStatement();

        public:
            Expression* get_condition();
            Statement* get_true_statements();
            Statement* get_false_statements();

            void set_condition(Expression* expression);
            void set_true_statements(Statement* stmt);
            void set_false_statements(Statement* stmt);

        private:
            Expression* condition;
            Statement* true_statements;
            Statement* false_statements;
    };
}

#endif
