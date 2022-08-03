#ifndef HAARD_AST_FOR_STATEMENT_H
#define HAARD_AST_FOR_STATEMENT_H

#include "ast/statement.h"
#include "ast/compound_statement.h"
#include "ast/expression.h"
#include "ast/expression_list.h"

namespace haard {
    class ForStatement : public Statement {
        public:
            ForStatement();
            ~ForStatement();

        public:
            ExpressionList* get_initialization();
            Expression* get_condition();
            ExpressionList* get_increment();
            CompoundStatement* get_statements();

            void add_initialization(Expression* expression);
            void set_condition(Expression* expression);
            void add_increment(Expression* expression);
            void set_statements(CompoundStatement* statements);

        private:
            Expression* condition;
            ExpressionList* init;
            ExpressionList* increment;
            CompoundStatement* statements;           
    };
}

#endif
