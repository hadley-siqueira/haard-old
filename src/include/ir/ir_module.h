#ifndef HAARD_IR_MODULE_H
#define HAARD_IR_MODULE_H

#include <vector>
#include "ir/ir_function.h"

namespace haard {
    class IRModule {
        public:
            IRModule();
            ~IRModule();

        public:
            void add_function(IRFunction* func);
            std::string to_cpp();

        private:
            std::vector<IRFunction*> functions;
    };
}

#endif
