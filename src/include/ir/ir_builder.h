#ifndef HAARD_IR_BUILDER_H
#define HAARD_IR_BUILDER_H

#include <vector>
#include "ast/ast.h"
#include "ir/ir.h"
#include "ir/ir_value.h"
#include "ir/ir_bin.h"
#include "ir/ir_unary.h"
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

            void build_address_of(UnOp* un);

            void build_binop(BinOp* bin, int kind);

            void build_literal(Literal* literal, int kind);

        public:
            void set_logger(Logger* logger);

        private:
            void add_instruction(IR* instruction);
            IRValue* new_temporary();

        private:
            int tmp_counter = 0;
            Logger* logger;
            IRValue* last_value;
            std::vector<IR*> instructions;
    };
}

#endif
