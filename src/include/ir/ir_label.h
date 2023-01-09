#ifndef HAARD_IR_LABEL_H
#define HAARD_IR_LABEL_H

#include "ir/ir.h"

namespace haard {
    class IRLabel : public IR {
        public:
            IRLabel();

        public:
            const char* get_label() const;
            void set_label(const char* value);

            int get_address() const;
            void set_address(int value);

    private:
            const char* label;
            int address;
    };
}

#endif
