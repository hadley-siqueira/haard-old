#ifndef HAARD_IR_ALLOCA_H
#define HAARD_IR_ALLOCA_H

#include "ir.h"
#include "ir_value.h"

namespace haard {
    class IRAlloca : public IR {
        public:
            IRAlloca();

        public:
            int get_size() const;
            void set_size(int value);

            int get_align() const;
            void set_align(int value);

            std::string get_name() const;
            void set_name(const std::string &value);

            IRValue* get_dst() const;
            void set_dst(IRValue* value);

    private:
            int size;
            int align;
            std::string name;
            IRValue* dst;
    };
}

#endif
