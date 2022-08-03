#ifndef HAARD_AST_FOR_STATEMENT_H
#define HAARD_AST_FOR_STATEMENT_H

#include "ast/statement.h"
#include "ast/compound_statement.h"
#include "ast/expression.h"

namespace haard {
    class ForStatement : public Statement {
        public:
            ForStatement();
            ~ForStatement();

        public:
            Expression* get_initialization();
            Expression* get_condition();
            Expression* get_increment();
            CompoundStatement* get_statements();

            void set_initialization(Expression* expression);
            void set_condition(Expression* expression);
            void set_increment(Expression* expression);
            void set_statements(CompoundStatement* statements);

        private:
            Expression* condition;
            Expression* init;
            Expression* increment;
            CompoundStatement* statements;           
    };
}

#endif
