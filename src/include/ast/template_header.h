#ifndef HAARD_AST_TEMPLATE_HEADER_H
#define HAARD_AST_TEMPLATE_HEADER_H

#include "ast/type.h"
#include "ast/type_list.h"

namespace haard {
    class TemplateHeader {
    public:
        TemplateHeader();

    public:
        void add_type(Type* type);
        Type* get_type(int idx);
        int types_count();
        TypeList* get_types();

        TemplateHeader* clone();
        std::string to_str();
        std::string get_qualified_name();

    private:
        TypeList* types;
    };
}

#endif
