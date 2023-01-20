#ifndef HAARD_AST_COMPOUND_STATEMENT_H
#define HAARD_AST_COMPOUND_STATEMENT_H

#include <vector>
#include "ast/statement.h"

namespace haard {
    class Scope;
    class CompoundStatement : public Statement {
        public:
            CompoundStatement();
            ~CompoundStatement();

        public:
            Statement* get_statement(int idx);
            int statements_count();
            void add_statement(Statement* stmt);

            Statement* clone();

            Scope* get_scope() const;
            void set_scope(Scope* value);

    private:
            std::vector<Statement*> statements;
            Scope* scope;
    };
}

#endif
