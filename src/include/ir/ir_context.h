#ifndef HAARD_IR_CONTEXT_H
#define HAARD_IR_CONTEXT_H

#include <map>
#include <vector>
#include "ast/ast.h"
#include "ir/ir_value.h"
#include "ir/ir_unary.h"
#include "ir/ir_bin.h"
#include "ir/ir.h"

namespace haard {
    class IRContext {
        public:
            IRContext();
            ~IRContext();

        public:
            IRBin* new_bin(int kind, IRValue* dst, IRValue* src1, IRValue* src2);
            IRUnary* new_unary(int kind,IRValue* dst, IRValue* src);
            IRValue* get_literal(int kind, const char* lexeme);
            IRValue* get_var(Identifier* id);
            IRValue* new_temporary();
            void add_instruction(IR* instruction);

            int instructions_count();
            IR* get_instruction(int i);

        private:
            int tmp_counter;
            std::map<std::string, IRValue*> values;
            std::map<std::string, IRValue*> alloca_map;
            std::vector<IR*> instructions;
    };
}

#endif
