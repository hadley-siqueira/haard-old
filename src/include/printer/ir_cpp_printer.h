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
            std::string get_function_name(std::string name);

        private:
            void print_binop(const char* oper, IRBin* bin);
            void print_headers();
            void print_main_function();
            void print_syscall_body();
            void print_function_temporaries(IRFunction* function);
            bool is_not_parameter(IRFunction* function, IRValue* value);
            void generate_functions_id(IRModules* modules);
            std::string generate_function_signature(IRFunction* function);

        private:
            void indent();
            void dedent();
            void print_indentation();

        private:
            int indent_c;
            std::stringstream* out;
            std::vector<IRValue*> temps;
            std::stringstream headers;
            std::stringstream functions;
            IRFunction* main_function;
            std::map<std::string, int> fmap;
            int function_counter;
    };
}

#endif
