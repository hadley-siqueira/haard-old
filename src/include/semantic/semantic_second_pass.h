#ifndef HAARD_SEMANTIC_SECOND_PASS_H
#define HAARD_SEMANTIC_SECOND_PASS_H

#include "ast/ast.h"
#include "semantic/semantic_pass.h"

namespace haard {
    class SemanticSecondPass : SemanticPass {
    public:
        void build_modules(Modules* modules);
        void build_module(Module* module);

        void build_class(Class* klass);

        void build_function(Function* function);
        void build_method(Function* method);

        void build_statement(Statement* stmt);
        void build_compound_statement(CompoundStatement* stmt);
        void build_expression_statement(ExpressionStatement* stmt);

        void build_expression(Expression* expr);

        void build_assignment(Assignment* expr);

        void build_plus(Plus* expr);

        void build_call(Call* expr);
        void build_method_call(Call* expr);
        void build_constructor_call(Call* expr);

        void build_expression_list(ExpressionList* list);

        void build_address_of(AddressOf* expr);
        void build_dereference(Dereference* expr);
        void build_dot(Dot* expr);

        void build_identifier(Identifier* expr);
        void build_literal_bool(Literal* expr);
        void build_literal_char(Literal* expr);
        void build_literal_integer(Literal* expr);
        void build_literal_float(Literal* expr);
        void build_literal_double(Literal* expr);

    private:
        bool is_new_variable(Assignment* expr);
        bool is_simple_call(Call* expr);
        bool is_method_call(Call* expr);
        bool is_constructor_call(Call* expr);
        void create_variable(Assignment* expr);
        int find_best_match(Symbol* sym, ExpressionList* args);
        bool compare_match(SymbolDescriptor* desc, ExpressionList* args);
        void build_simple_call(Call* expr);
        void set_call_type(Call* expr, Symbol* sym, int idx);

    };
}

#endif
