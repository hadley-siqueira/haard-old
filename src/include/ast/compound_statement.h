#ifndef HAARD_AST_COMPOUND_STATEMENT_H
#define HAARD_AST_COMPOUND_STATEMENT_H

#include <vector>
#include "ast/statement.h"

namespace haard {
    class CompoundStatement : public Statement {
        public:
            CompoundStatement();
            ~CompoundStatement();

        public:
            Statement* get_statement(int idx);
            int statements_count();
            void add_statement(Statement* stmt);

            Statement* clone();

        private:
            std::vector<Statement*> statements;
    };
}

#endif
