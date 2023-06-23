#ifndef HAARD_SEMANTIC_SECOND_PASS_H
#define HAARD_SEMANTIC_SECOND_PASS_H

#include "ast/ast.h"
#include "semantic/semantic_pass.h"

namespace haard {
    class SemanticSecondPass : SemanticPass {
    public:
        void build_modules(Modules* modules);
        void build_module(Module* module);

        void build_function(Function* function);

        void build_statement(Statement* stmt);
        void build_compound_statement(CompoundStatement* stmt);
        void build_expression_statement(ExpressionStatement* stmt);

        void build_expression(Expression* expr);

        void build_assignment(Assignment* expr);

        void build_identifier(Identifier* expr);

    private:
        bool is_new_variable(Assignment* expr);
        void create_variable(Assignment* expr);

    };
}

#endif
