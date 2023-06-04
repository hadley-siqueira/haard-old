#ifndef HAARD_AST_UNOP_H
#define HAARD_AST_UNOP_H

#include "token/token.h"
#include "ast/expression.h"

namespace haard {
    class UnOp : public Expression {
        public:
            UnOp();
            UnOp(int kind, Expression* expression=nullptr);
            UnOp(int kind, Token& token, Expression* expression=nullptr);
            virtual ~UnOp();

        public:
            Expression* get_expression();

            void set_expression(Expression* expression);
            Expression* clone();

        private:
            Expression* expression;
            int line;
            int column;
    };
}

#endif
