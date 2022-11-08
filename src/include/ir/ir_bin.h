#ifndef HAARD_IR_BIN_H
#define HAARD_IR_BIN_H

#include "ir.h"
#include "ir_value.h"

namespace haard {
    class IRBin : public IR {
        public:
            IRBin(int kind, IRValue* dst=nullptr, IRValue* src1=nullptr, IRValue* src2=nullptr);
            ~IRBin();

            std::string to_str();
            std::string to_cpp();

        private:
            std::string to_str1(std::string op);
            std::string to_cpp1(std::string op);

        private:
            IRValue* dst;
            IRValue* src1;
            IRValue* src2;
    };
}

#endif
