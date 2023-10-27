#ifndef HAARD_IR_CPP_PRINTER_H
#define HAARD_IR_CPP_PRINTER_H

#include <sstream>
#include <map>
#include "ir/ir_modules.h"
#include "ir/ir_module.h"
#include "ir/ir_function.h"

namespace haard {
    class IRCppPrinter {
        public:
            IRCppPrinter();

        public:
            std::string get_output();
            void print_modules(IRModules* modules);
            void print_module(IRModule* module);
            void print_function(IRFunction* function);
            void print_function_body(IRFunction* function);
            void print_instruction(IR* ir);
            std::string get_function_name(IRFunction* function);

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
            IRFunction* main_function;
            std::map<std::string, int> fmap;
            int function_counter;
    };
}

#endif
