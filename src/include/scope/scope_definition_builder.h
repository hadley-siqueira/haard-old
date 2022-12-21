#ifndef HAARD_SCOPE_DEFINITION_BUILDER_H
#define HAARD_SCOPE_DEFINITION_BUILDER_H

#include <stack>
#include "ast/ast.h"
#include "log/logger.h"
#include "scope/scope.h"

namespace haard {
    class ScopeDefinitionBuilder {
        public:
            ScopeDefinitionBuilder();

        public:
            void build(Sources* sources);

        private:
            void build_sources(Sources* sources);
            void build_source(Source* source);

            void build_class(Class* klass);
            void build_class_methods(Class* klass);

            void build_function(Function* function);

            void build_statement(Statement* statement);
            void build_compound_statement(CompoundStatement* stmts);
            void build_expression_statement(ExpressionStatement* statement);
            void build_jump_statement(JumpStatement* statement);
            void build_while_statement(WhileStatement* statement);
            void build_for_statement(ForStatement* statement);
            void build_branch_statement(BranchStatement* statement);

            void build_expression(Expression* expression);
            void build_identifier(Identifier* id);
            void build_assignment(BinOp* bin);
            void build_call(BinOp* bin);

            void build_pre_inc(UnOp* op);
            void build_pre_dec(UnOp* op);
            void build_pos_inc(UnOp* op);
            void build_pos_dec(UnOp* op);

            void build_plus(BinOp* bin);
            void build_minus(BinOp* bin);

            void build_greater_than(BinOp* bin);
            void build_less_than(BinOp* bin);
            void build_greater_or_equal_than(BinOp* bin);
            void build_less_or_equal_than(BinOp* bin);

            void build_equal(BinOp* bin);
            void build_not_equal(BinOp* bin);

            void build_binop(BinOp* bin);
            void build_relational(BinOp* bin);

            void build_address_of(UnOp* op);
            void build_dereference(UnOp* op);

            void build_literal(Literal* literal, int kind);
            void build_expression_list(ExpressionList* exprlist);

        private:
            void define_sources(Sources* sources);
            void define_sources_classes(Sources* sources);
            void define_sources_functions(Sources* sources);
            void define_source_classes(Source* source);
            void define_source_functions(Source* source);

            void define_class(Class* klass);
            void define_class_variables(Class* klass);
            void define_class_variable(Variable* var);
            void define_class_template_header(Class* klass);
            void define_class_super(Class* klass);

            void define_class_methods(Class* klass);
            void define_class_method(Function* method);
            void define_method_signature(Function* method);

            void define_function(Function* function);
            void define_function_signature(Function* function);
            void define_function_template_header(Function* function);
            void define_function_parameters(Function* function);
            void define_function_self_type(Function* function);
            void define_overloaded_function(Symbol* symbol, Function* function);

            void connect_sibling_scopes(Sources* sources);

        private:
            void link_type(Type* type);
            void link_named_type(NamedType* type);
            void link_function_type(FunctionType* type);
            void link_template_header(TemplateHeader* header);

        private:
            bool is_new_var_assign(BinOp* bin);
            void create_new_var(BinOp* bin);

        public:
            void set_logger(Logger* logger);

        private:
            void enter_scope(Scope* scope);
            void leave_scope();

        private:
            Logger* logger;
            Scope* current_scope;
            std::stack<Scope*> scopes;

            Function* current_function;
            Class* current_class;

            int var_counter;
            int function_counter;
            int class_counter;
    };
}

#endif
