#ifndef HAARD_IR_FUNCTION_H
#define HAARD_IR_FUNCTION_H

#include <vector>
#include <string>
#include "ir/ir_context.h"

namespace haard {
    class IRFunction : public IR {
        public:
            IRFunction();
            ~IRFunction();

        public:
            void set_name(std::string name);
            IRContext* get_context();
            std::string to_str();
            std::string to_cpp();

        private:
            std::string name;
            std::vector<IRValue*> params;
            IRContext* ctx;
    };
}

#endif
