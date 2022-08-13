#ifndef HAARD_AST_FUNCTION_EXPRESSION_H
#define HAARD_AST_FUNCTION_EXPRESSION_H

#include "ast/function.h"
#include "ast/expression.h"

namespace haard {
    class FunctionExpression : public Expression {
        public:
            FunctionExpression(Function* function);
            ~FunctionExpression();

        public:
            Function* get_function();
            Expression* clone();

        private:
            Function* function;
    };
}

#endif
