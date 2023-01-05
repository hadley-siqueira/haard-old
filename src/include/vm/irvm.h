#ifndef HAARD_IR_VM_H
#define HAARD_IR_VM_H

#include <map>
#include <stack>
#include <cstdint>
#include "ir/ir.h"
#include "ir/ir_function.h"

namespace haard {
    class IrVM {
        public:
            IrVM();
            void execute_function(IRFunction* function);
            void execute(IR* ir);

            void dump_memory();
            void dump_values();

        private:
            uint64_t load64(uint64_t addr);
            void store64(uint64_t addr, uint64_t value);

            void save_context();
            void restore_context();

        private:
            std::stack<std::map<std::string, uint64_t>> values_stack;
            std::stack<uint64_t> ip_stack;
            std::map<std::string, uint64_t> current_values;
            uint64_t sp;
            uint64_t ip;
            uint8_t* mem;
    };
}

#endif
