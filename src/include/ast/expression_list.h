#ifndef HAARD_AST_EXPRESSION_LIST_H
#define HAARD_AST_EXPRESSION_LIST_H

#include <vector>
#include "ast/expression.h"

namespace haard {
    class ExpressionList : public Expression {
        public:
            ExpressionList(int kind);
            ExpressionList(int kind, Expression* expression);
            ExpressionList(int kind, std::vector<Expression*> exprs);
            ~ExpressionList();

        public:
            std::string to_str();
            std::string to_str(std::string begin, std::string end);

            Expression* get_expression(int idx);

            void add_expression(Expression* expression);
            int expressions_count();

            Expression* clone();
            
        private:
            std::vector<Expression*> expressions;
    };
}

#endif
