#ifndef HAARD_IR_MODULES_H
#define HAARD_IR_MODULES_H

#include <vector>
#include "ir/ir_function.h"
#include "ir/ir_module.h"

namespace haard {
    class IRModules {
        public:
            IRModules();
            ~IRModules();

        public:
            void add_module(IRModule* module);
            int modules_count();

            IRFunction* get_function(const char* name);
            IRFunction* get_main_function() const;
            /*
            void add_function(IRFunction* func);

            IRFunction* get_function(int i);



            void set_main_function(IRFunction* value);*/

        private:
            std::vector<IRModule*> modules;
            IRFunction* main_function;

    };
}

#endif
