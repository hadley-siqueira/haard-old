#ifndef HAARD_SCOPE_BUILDER_H
#define HAARD_SCOPE_BUILDER_H

#include <stack>
#include "ast/ast.h"
#include "scope/scope.h"

namespace haard {
    class ScopeBuilder {
        public:
            ScopeBuilder();

        public:
            void build_sources(Sources* sources);
            void build_source(Source* source);

            void build_class(Class* klass);
            void build_method(Function* func);

            void build_function(Function* func);
            void build_function_parameters(Function* func);
            void build_parameter(Variable* var);
            void build_class_variable(Variable* var);

            void build_type(Type* type);
            void build_named_type(NamedType* type);
            void build_indirection_type(IndirectionType* type);
            void build_array_list_type(ArrayListType* type);

            void build_statement(Statement* statement);
            void build_compound_statement(CompoundStatement* stmts);
            void build_expression_statement(ExpressionStatement* statement);
            void build_jump_statement(JumpStatement* statement);
            void build_while_statement(WhileStatement* statement);
            void build_for_statement(ForStatement* statement);
            void build_branch_statement(BranchStatement* statement);

            void build_expression(Expression* expression);
            void build_assignment(BinOp* bin);

            void build_address_of(UnOp* op);
            void build_dot(BinOp* bin);
            void build_call_expression(BinOp* bin);

            void build_binop(std::string oper, BinOp* bin);
            void build_unop(std::string oper, UnOp* un, bool before=true);
            void build_identifier(Identifier* id);
            void build_literal(Literal* literal, int kind);
            void build_expression_list(ExpressionList* tuple);
            void build_hash(ExpressionList* hash);
            void build_function_expression(FunctionExpression* function);
            void build_new_expression(NewExpression* expr);

        private:
            void define_sources_elements(Sources* sources);
            void define_source_elements(Source* source);
            void define_class(Class* klass);
            void define_function(Function* func);
            void define_function_template_list(Function* func);
            void define_function_parameters(Function* func);
            void define_function_self_type(Function* func);
            void define_overloaded_function(Symbol* sym, Function* func);
            void define_method(Function* func);

            bool is_function_call(Expression* expr);
            bool is_constructor_call(Expression* expr);

        private:
            void build_class_variables(Class* klass);
            void build_class_methods(Class* klass);

        private:
            void enter_scope(Scope* scope);
            void leave_scope();

        private:
            Scope* current_scope;
            Function* current_function;
            Class* current_class;
            std::stack<Scope*> scopes;
            int class_counter;
            int function_counter;
            int method_counter;
            int var_counter;
    };
}

#endif
