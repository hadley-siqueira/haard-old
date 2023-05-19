#ifndef HAARD_ASSIGNMENT_SCOPE_BUILDER_H
#define HAARD_ASSIGNMENT_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class AssignmentScopeBuilder : public ScopeBuilder {
    public:
        AssignmentScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_assignment(BinOp* bin);

    private:
        bool is_new_var_assignment(BinOp* bin);
        void create_new_var(BinOp* bin);

    };
}

#endif
