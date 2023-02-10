#ifndef HAARD_AST_TUPLE_TYPE_H
#define HAARD_AST_TUPLE_TYPE_H

#include "ast/type.h"
#include "ast/type_list.h"

namespace haard {
    class TupleType : public Type {
    public:
        TupleType();
        ~TupleType();

    public:
        void add_type(Type* type);
        Type* get_type(int idx);
        int types_count();
        TypeList* get_types();

        bool equal(Type* type);
        Type* clone();
        std::string to_str();
        std::string get_qualified_name();

    private:
        TypeList* types;
    };
}

#endif
