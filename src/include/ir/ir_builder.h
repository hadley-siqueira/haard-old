#ifndef HAARD_IR_BUILDER_H
#define HAARD_IR_BUILDER_H

#include <vector>
#include <map>
#include "ast/ast.h"
#include "ir/ir.h"
#include "ir/ir_value.h"
#include "ir/ir_bin.h"
#include "ir/ir_unary.h"
#include "ir/ir_call.h"
#include "ir/ir_context.h"
#include "ir/ir_function.h"
#include "ir/ir_module.h"
#include "ir/ir_modules.h"
#include "log/logger.h"

namespace haard {
    class IRBuilder {
        public:
            IRBuilder();
            ~IRBuilder();

        public:
            void build(Sources* sources);
            void build_source(Source* source);
            void build_class(Class* klass);

            void build_function(Function* function);
            void build_function_parameters(Function* function, IRFunction* ir_func);
            void build_function_body(Function* function);

            void build_statement(Statement* statement);
            void build_compound_statement(CompoundStatement* stmts);
            void build_expression_statement(ExpressionStatement* statement);
            void build_jump_statement(JumpStatement* statement);
            void build_while_statement(WhileStatement* statement);

            void build_for_statement(ForStatement* statement);
            void build_for_init(ForStatement* statement);
            void build_for_inc(ForStatement* statement);

            void build_branch_statement(BranchStatement* statement);
            void build_variable_declaration(VarDeclaration* statement);

            void build_if(BranchStatement* statement);
            void build_else(BranchStatement* statement);
            void build_return_statement(JumpStatement* statement);

            void build_expression(Expression* expression, bool lvalue=false);

            void build_scope(BinOp* bin, bool lvalue=false);

            void build_identifier(Identifier* id, bool lvalue=false);
            void build_identifier_lvalue(Identifier* id);
            void build_identifier_rvalue(Identifier* id);

            void build_pre_inc(UnOp* un);
            void build_cast(CastExpression* cast);

            void build_call(BinOp* bin);
            void build_call_arguments(IRCall* call, ExpressionList* args, IRValue *this_ptr=nullptr);
            void build_function_call(BinOp* bin, IRCall* call);
            void build_method_call(BinOp* bin, IRCall* call);

            void build_index_access(BinOp* bin, bool lvalue=false);
            void build_member_access(BinOp* bin, bool lvalue=false);

            void build_assignment(BinOp* bin, bool lvalue=false);
            void build_constructor_assignment(BinOp* bin);

            void build_logical_or(BinOp* bin);
            void build_logical_and(BinOp* bin);

            void build_equal(BinOp* bin);
            void build_not_equal(BinOp* bin);

            void build_less_than(BinOp* bin);
            void build_greater_than(BinOp* bin);
            void build_less_than_or_equal(BinOp* bin);
            void build_greater_than_or_equal(BinOp* bin);

            void build_plus(BinOp* bin);
            void build_minus(BinOp* bin);

            void build_times(BinOp* bin);
            void build_division(BinOp* bin);
            void build_modulo(BinOp* bin);

            void build_bitwise_or(BinOp* bin);
            void build_bitwise_xor(BinOp* bin);
            void build_bitwise_and(BinOp* bin);

            void build_sll(BinOp* bin);
            void build_srl(BinOp* bin);
            void build_sra(BinOp* bin);

            void build_address_of(UnOp* op);
            void build_dereference(UnOp* op, bool lvalue=false);

            void build_binop(BinOp* bin, int kind);

            void build_literal(Literal* literal, int kind);
            void build_literal_integer(Literal* literal);
            void build_literal_string(Literal* literal);
            void build_string_builder(StringBuilder* sb);

            void build_sizeof(UnOp* un);

        private:
            bool is_function_call(BinOp* bin);
            bool is_method_call(BinOp* bin);
            bool is_constructor_call(BinOp* bin);

        public:
            void set_logger(Logger* logger);

            IRModules* get_modules() const;

    private:
            Logger* logger;
            IRValue* last_value;
            IRContext* ctx;
            IRModules* modules;
            IRModule* current_module;
    };
}

#endif
