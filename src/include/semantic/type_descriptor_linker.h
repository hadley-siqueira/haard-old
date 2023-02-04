#ifndef HAARD_TYPE_DESCRIPTOR_LINKER_H
#define HAARD_TYPE_DESCRIPTOR_LINKER_H

#include "ast/ast.h"
#include "ast/source.h"
#include "log/logger.h"

namespace haard {
    class TypeDescriptorLink {
    public:
        TypeDescriptorLink(Scope* scope=nullptr, Logger* logger=nullptr);

    public:
        void link_type(Type* type);
        void link_named_type(NamedType* type);
        void link_pointer_type(IndirectionType* type);
        void link_reference_type(IndirectionType* type);
        void link_function_type(FunctionType* type);
        void link_array_type(ArrayListType* type);

    public:
        Scope* get_current_scope() const;
        Logger* get_logger() const;

        void set_current_scope(Scope* value);
        void set_logger(Logger* value);

    private:
        Scope* current_scope;
        Logger* logger;
    };
}

#endif
