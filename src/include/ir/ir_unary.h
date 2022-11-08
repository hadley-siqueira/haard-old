#ifndef HAARD_IR_UNARY_H
#define HAARD_IR_UNARY_H

#include "ir.h"
#include "ir_value.h"

namespace haard {
    class IRUnary : public IR {
        public:
            IRUnary(int kind, IRValue* dst=nullptr, IRValue* src=nullptr);
            ~IRUnary();

        public:
            std::string to_str();
            std::string to_cpp();

        private:
            IRValue* dst;
            IRValue* src;
    };
}

#endif
