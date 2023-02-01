#ifndef HAARD_AST_DATA_H
#define HAARD_AST_DATA_H

#include <vector>
#include "ast/type_declaration.h"
#include "ast/data_field.h"

namespace haard {
    class Data : public TypeDeclaration {
        public:
            Data();
    };
}

#endif
