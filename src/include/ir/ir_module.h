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
            int functions_count();
            IRFunction* get_function(int i);
            IRFunction* get_function(const char* name);

            IRFunction* get_main_function() const;
            void set_main_function(IRFunction* value);

        private:
            std::vector<IRFunction*> functions;
            std::map<std::string, IRFunction*> functions_map;
            IRFunction* main_function;
    };
}

#endif
