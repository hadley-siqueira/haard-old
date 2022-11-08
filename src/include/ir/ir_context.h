#ifndef HAARD_IR_CONTEXT_H
#define HAARD_IR_CONTEXT_H

#include <map>
#include <list>
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

            std::string to_str();
            std::string to_cpp();

            void debug();

        private:
            int tmp_counter;
            std::map<std::string, IRValue*> values;
            std::map<std::string, IRValue*> alloca_map;
            std::list<IR*> instructions;
    };
}

#endif
