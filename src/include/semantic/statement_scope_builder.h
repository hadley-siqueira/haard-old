#ifndef HAARD_STATEMENT_SCOPE_BUILDER_H
#define HAARD_STATEMENT_SCOPE_BUILDER_H

#include "ast/ast.h"

namespace haard {
    class StatementScopeBuilder {
    public:
        void build_compound_statement(CompoundStatement* stmt);
    };
}

#endif
