#ifndef HAARD_IR_PRINTER_H
#define HAARD_IR_PRINTER_H

#include <sstream>
#include "ir/ir_modules.h"
#include "ir/ir_module.h"
#include "ir/ir_function.h"

namespace haard {
    class IRPrinter {
        public:
            IRPrinter();

        public:
            void print_modules(IRModules* modules);
            void print_module(IRModule* module);
            void print_function(IRFunction* function);
            void print_function_body(IRFunction* function);
            void print_instruction(IR* ir);

        private:
            void print_binop(const char* oper, IRBin* bin);

        private:
            void indent();
            void dedent();
            void print_indentation();

            private:
                int indent_c;
                std::stringstream* out;
                std::stringstream functions;
    };
}

#endif
