#ifndef HAARD_PRINTER_H
#define HAARD_PRINTER_H

#include <sstream>
#include "ast/ast.h"

namespace haard {
    class Printer {
        public:
            std::string to_str();
            void print_source(Source* source);
            void print_import(Import* import);

        private:
            std::stringstream out;
    };
}

#endif
