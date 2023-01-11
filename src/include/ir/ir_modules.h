#ifndef HAARD_IR_MODULES_H
#define HAARD_IR_MODULES_H

#include <vector>
#include <map>
#include "ir/ir_function.h"
#include "ir/ir_module.h"

namespace haard {
    class IRModules {
        public:
            IRModules();
            ~IRModules();

        public:
            void add_module(IRModule* module);
            IRModule* get_module(int i);
            int modules_count();

            IRFunction* get_function(const char* name);
            IRFunction* get_main_function() const;

            void add_string_literal(std::string s);
            int strings_count();
            std::string get_string(int i);

        private:
            std::vector<IRModule*> modules;
            IRFunction* main_function;
            std::map<std::string, int> string_pool_map;
            std::vector<std::string> string_pool;
            int string_counter;

    };
}

#endif
