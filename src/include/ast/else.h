#ifndef HAARD_AST_ELSE_H
#define HAARD_AST_ELSE_H

#include "ast/statement.h"
#include "ast/expression.h"

namespace haard {
    class Else : public Statement {
    public:
        Else();
        ~Else();

    public:
        Statement* get_statements() const;
        void set_statements(Statement* value);

    private:
        Expression* condition;
        Statement* statements;
    };
}

#endif
