#ifndef HAARD_IR_VM_H
#define HAARD_IR_VM_H

#include <map>
#include <stack>
#include <cstdint>
#include "ir/ir.h"
#include "ir/ir_function.h"
#include "ir/ir_module.h"
#include "ir/ir_modules.h"
#include "vm/irvm_context.h"

namespace haard {
    class IrVM {
        public:
            IrVM();

        public:
            void execute_modules(IRModules *module);
            void execute_function(IRFunction* function);
            void execute(IR* ir);

            void dump_memory(int n_bytes=40);
            void dump_values();

        private:
            uint64_t load64(uint64_t addr);
            uint64_t load32(uint64_t addr);
            uint64_t load16(uint64_t addr);
            uint64_t load8(uint64_t addr);

            bool is_special_load_address(uint64_t addr);
            bool is_special_store_address(uint64_t addr);
            uint64_t load_special_address(uint64_t addr);
            void store_special_address(uint64_t addr, uint64_t value);

            void store64(uint64_t addr, uint64_t value);
            void store32(uint64_t addr, uint64_t value);
            void store16(uint64_t addr, uint64_t value);
            void store8(uint64_t addr, uint64_t value);

            void save_context();
            void restore_context();

        private:
            std::stack<IrVMContext> context_stack;
            std::map<std::string, uint64_t> values;
            uint64_t sp;
            uint64_t ip;
            uint64_t fp;
            uint8_t* mem;
            std::vector<uint64_t> args;
            uint64_t return_value;
            IRModules* modules;

            int string_addr_counter;
            std::map<std::string, uint64_t> string_address_map;
    };
}

#endif
