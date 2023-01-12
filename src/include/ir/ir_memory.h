#ifndef HAARD_IR_MEMORY_H
#define HAARD_IR_MEMORY_H

#include "ir.h"
#include "ir_value.h"

namespace haard {
    class IRMemory : public IR {
        public:
            IRMemory(int kind, IRValue* dst=nullptr, IRValue* src=nullptr, int offset=0);
            ~IRMemory();

        public:
            IRValue* get_dst() const;
            void set_dst(IRValue* value);

            IRValue* get_src() const;
            void set_src(IRValue* value);

            int get_offset() const;
            void set_offset(int value);

        private:
            IRValue* dst;
            IRValue* src;
            int offset;
    };
}

#endif
