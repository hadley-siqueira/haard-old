#ifndef HAARD_IR_BRANCH_H
#define HAARD_IR_BRANCH_H

#include "ir.h"
#include "ir_value.h"

namespace haard {
    class IRBranch : public IR {
        public:
            IRBranch(int kind, IRValue* src=nullptr, IRValue* label=nullptr);
            ~IRBranch();

            IRValue* get_label() const;
            void set_label(IRValue* value);

            IRValue* get_src1() const;
            void set_src1(IRValue* value);

            IRValue* get_src2() const;
            void set_src2(IRValue* value);

        private:
            IRValue* label;
            IRValue* src1;
            IRValue* src2;
    };
}

#endif
