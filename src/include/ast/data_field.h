#ifndef HAARD_AST_DATAFIELD_H
#define HAARD_AST_DATAFIELD_H

#include <string>
#include "ast/field.h"

namespace haard {
    class DataField : public Field {
        public:
            virtual bool is_single();
            virtual bool is_compound();

    };
}

#endif
