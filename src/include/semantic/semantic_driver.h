#ifndef HAARD_SEMANTIC_DRIVER_H
#define HAARD_SEMANTIC_DRIVER_H

#include "ast/ast.h"

namespace haard {
    class SemanticDriver {
    public:
        void build_modules(Modules* modules);

    private:
        void first_pass(Modules* modules);
        void second_pass(Modules* modules);
    };
}

#endif
