#ifndef HAARD_IR_CONTEXT_H
#define HAARD_IR_CONTEXT_H

#include <map>
#include <list>
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
            IRValue* new_temporary();
            void debug();

        private:
            int tmp_counter;
            std::map<std::string, IRValue*> values;
            std::list<IR*> instructions;
    };
}

#endif
