#ifndef HAARD_PRINTER_H
#define HAARD_PRINTER_H

#include <sstream>
#include "ast/ast.h"

namespace haard {
    class Printer {
        public:
            Printer();

        public:
            std::string to_str();
            void print_source(Source* source);
            void print_import(Import* import);
            void print_function(Function* function);
            void print_parameters(Function* function);
            void print_type(Type* type);

        private:
            void indent();
            void dedent();
            void print_indentation();

        private:
            int indent_c;
            std::stringstream out;
    };
}

#endif
