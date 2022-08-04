#ifndef HAARD_AST_VAR_DECL_H
#define HAARD_AST_VAR_DECL_H

#include "ast/statement.h"
#include "ast/variable.h"
#include "ast/expression.h"

namespace haard {
    class VarDeclaration : public Statement {
        public:
            VarDeclaration();
            ~VarDeclaration();

            Variable* get_variable();
            Expression* get_expression();

            void set_variable(Variable* var);
            void set_expression(Expression* expr);

        private:
            Variable* var;
            Expression* expr;
    };
}

#endif
