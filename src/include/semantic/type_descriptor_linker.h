#ifndef HAARD_TYPE_DESCRIPTOR_LINKER_H
#define HAARD_TYPE_DESCRIPTOR_LINKER_H

#include "ast/ast.h"
#include "ast/module.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class TypeDescriptorLink : public ScopeBuilder {
    public:
        TypeDescriptorLink(ScopeBuilderContext* context=nullptr);

    public:
        void link_type(Type* type);
        void link_named_type(NamedType* type);
        void link_pointer_type(IndirectionType* type);
        void link_reference_type(IndirectionType* type);
        void link_function_type(FunctionType* type);
        void link_array_type(ArrayListType* type);
        void link_tuple_type(TupleType* type);

        void link_type_list(TypeList* types);
    };
}

#endif
