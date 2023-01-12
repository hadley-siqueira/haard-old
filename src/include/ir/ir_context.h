#ifndef HAARD_IR_CONTEXT_H
#define HAARD_IR_CONTEXT_H

#include <map>
#include <vector>
#include "ast/ast.h"
#include "ir/ir_value.h"
#include "ir/ir_unary.h"
#include "ir/ir_bin.h"
#include "ir/ir_label.h"
#include "ir/ir_alloca.h"
#include "ir/ir_memory.h"
#include "ir/ir.h"

namespace haard {
    class IRContext {
        public:
            IRContext();
            ~IRContext();

        public:
            IRBin* new_bin(int kind, IRValue* dst, IRValue* src1, IRValue* src2);
            IRUnary* new_unary(int kind,IRValue* dst, IRValue* src);
            IRAlloca* new_alloca(std::string name, int size=8, int align=8);
            IRMemory* new_load(int size, IRValue* src);
            IRMemory* new_store(int size, IRValue* dst, IRValue* src);
            IRBin* new_binary(int kind, IRValue* src1, IRValue* src2);

            IRValue* get_literal(int kind, std::string lexeme);
            IRValue* new_temporary();
            IRValue* new_label_value(std::string value);
            IRLabel* new_label();
            void add_instruction(IR* instruction);

            int instructions_count();
            IR* get_instruction(int i);

            void set_alloca_value(std::string name, IRValue* value);
            IRValue* get_alloca_value(std::string name);
            bool has_alloca(std::string name);

        private:
            int tmp_counter;
            int label_counter;
            std::map<std::string, IRValue*> values;
            std::map<std::string, IRValue*> alloca_map;
            std::vector<IR*> instructions;
    };
}

#endif
