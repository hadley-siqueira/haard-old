#ifndef HAARD_AST_TEMPLATE_HEADER_H
#define HAARD_AST_TEMPLATE_HEADER_H

#include <string>
#include <vector>
#include "ast/type.h"

namespace haard {
    class TemplateHeader {
        public:
            void add_type(Type* type);
            int types_count();
            Type* get_type(int idx);
            TemplateHeader* clone();
            std::string to_str();

        private:
            std::vector<Type*> types;        
    };
}

#endif
