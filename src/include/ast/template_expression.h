#ifndef HAARD_AST_TEMPLATE_EXPRESSION_H
#define HAARD_AST_TEMPLATE_EXPRESSION_H

#include "ast/expression.h"
#include "ast/type_list.h"

namespace haard {
    class TemplateExpression : public Expression {
        public:
            TemplateExpression();
            TemplateExpression(Expression* expr, TypeList* types);
            ~TemplateExpression();

        public:
            void set_expression(Expression* expression);
            Expression* get_expression();

            void set_types(TypeList* types);
            TypeList* get_types();

            Expression* clone();

        private:
            Expression* expression;
            TypeList* types;
    };
}

#endif
