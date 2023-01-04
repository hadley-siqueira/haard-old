#ifndef HAARD_IR_BIN_H
#define HAARD_IR_BIN_H

#include "ir.h"
#include "ir_value.h"

namespace haard {
    class IRBin : public IR {
        public:
            IRBin(int kind, IRValue* dst=nullptr, IRValue* src1=nullptr, IRValue* src2=nullptr);
            ~IRBin();

            IRValue* get_dst() const;
            void set_dst(IRValue* value);

            IRValue* get_src1() const;
            void set_src1(IRValue* value);

            IRValue* get_src2() const;
            void set_src2(IRValue* value);

    private:


    private:
            IRValue* dst;
            IRValue* src1;
            IRValue* src2;
    };
}

#endif
