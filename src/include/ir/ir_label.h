#ifndef HAARD_IR_LABEL_H
#define HAARD_IR_LABEL_H

#include "ir/ir.h"

namespace haard {
    class IRLabel : public IR {
        public:
            IRLabel();

        public:
            std::string get_label() const;
            void set_label(std::string value);

    private:
            std::string label;
    };
}

#endif
