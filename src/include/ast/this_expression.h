#ifndef HAARD_AST_THIS_H
#define HAARD_AST_THIS_H

#include "ast/expression.h"
#include "ast/expression_list.h"

namespace haard {
    class ThisExpression : public Expression {
        public:
            ThisExpression(Token& token);
            ThisExpression();
            ~ThisExpression();

        public:
            Expression* clone();

        private:
    };
}

#endif
