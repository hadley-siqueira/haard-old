#ifndef HAARD_IR_UNARY_H
#define HAARD_IR_UNARY_H

#include "ir.h"
#include "ir_value.h"

namespace haard {
    class IRUnary : public IR {
        public:
            IRUnary(int kind, IRValue* dst=nullptr, IRValue* src=nullptr);
            ~IRUnary();

            IRValue* get_dst() const;
            void set_dst(IRValue *value);

            IRValue* get_src() const;
            void set_src(IRValue *value);

    private:
            IRValue* dst;
            IRValue* src;
    };
}

#endif
