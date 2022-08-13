#ifndef HAARD_AST_JUMP_STATEMENT_H
#define HAARD_AST_JUMP_STATEMENT_H

#include "ast/statement.h"
#include "ast/expression.h"

namespace haard {
    class JumpStatement : public Statement {
        public:
            JumpStatement(int kind);
            ~JumpStatement();

        public:
            Expression* get_expression();
            void set_expression(Expression* expression);

            Statement* clone();

        private:
            Expression* expression;
    };
}

#endif
