#ifndef HAARD_AST_EXPRESSION_STATEMENT_H
#define HAARD_AST_EXPRESSION_STATEMENT_H

#include "ast/statement.h"
#include "ast/expression.h"

namespace haard {
    class ExpressionStatement : public Statement {
        public:
            ExpressionStatement(Expression* expression=nullptr);
            ~ExpressionStatement();

        public:
            Expression* get_expression();

            void set_expression(Expression* expression);

        private:
            Expression* expression;
    };
}

#endif
