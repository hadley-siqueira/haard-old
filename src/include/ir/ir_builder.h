#ifndef HAARD_IR_BUILDER_H
#define HAARD_IR_BUILDER_H

#include <vector>
#include <map>
#include "ast/ast.h"
#include "ir/ir.h"
#include "ir/ir_value.h"
#include "ir/ir_bin.h"
#include "ir/ir_unary.h"
#include "ir/ir_context.h"
#include "ir/ir_function.h"
#include "ir/ir_module.h"
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

            void build_statement(Statement* statement);
            void build_compound_statement(CompoundStatement* stmts);
            void build_expression_statement(ExpressionStatement* statement);
            void build_jump_statement(JumpStatement* statement);
            void build_while_statement(WhileStatement* statement);
            void build_for_statement(ForStatement* statement);
            void build_branch_statement(BranchStatement* statement);

            void build_expression(Expression* expression, bool lvalue=false);
            void build_identifier(Identifier* id, bool lvalue=false);

            void build_assignment(BinOp* bin, bool lvalue=false);

            void build_plus(BinOp* bin);
            void build_minus(BinOp* bin);

            void build_times(BinOp* bin);
            void build_division(BinOp* bin);

            void build_address_of(UnOp* op);
            void build_dereference(UnOp* op, bool lvalue=false);

            void build_binop(BinOp* bin, int kind);

            void build_literal(Literal* literal, int kind);

        public:
            void set_logger(Logger* logger);

        private:
            Logger* logger;
            IRValue* last_value;
            IRContext* ctx;
            std::map<std::string, IRValue*> alloca_map;
            std::vector<IRFunction*> functions;
            std::vector<IRModule*> modules;
            IRModule* current_module;
    };
}

#endif
