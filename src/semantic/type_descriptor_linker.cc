#include <iostream>
#include "semantic/type_descriptor_linker.h"
#include "log/actions.h"

using namespace haard;

TypeDescriptorLink::TypeDescriptorLink() {

}

TypeDescriptorLink::TypeDescriptorLink(Scope* scope) {
    this->scope = scope;
}

void TypeDescriptorLink::link_type(Type* type) {
    if (type == nullptr) {
        return;
    }

    switch (type->get_kind()) {
    case TYPE_NAMED:
        link_named_type((NamedType*) type);
        break;

    case TYPE_POINTER:
        link_pointer_type((IndirectionType*) type);
        break;

    case TYPE_REFERENCE:
        link_reference_type((IndirectionType*) type);
        break;

    case TYPE_FUNCTION:
        link_function_type((FunctionType*) type);
        break;

    case TYPE_ARRAY:
        link_array_type((ArrayListType*) type);
        break;

    case TYPE_TUPLE:
        link_tuple_type((TupleType*) type);
        break;
    }
}

void TypeDescriptorLink::link_named_type(NamedType* type) {
    std::string name = type->get_name();
    Scope* scope = this->scope;
    Symbol* sym = scope->resolve(name);

    if (!sym) {
        std::string msg;

        msg = "type <white>'" + name + "'</white> not in scope.\n";
        msg += "Current scope is:\n";
        msg += scope->debug();
        log_error_and_exit(msg);
    }

    SymbolDescriptor* desc = sym->get_descriptor(0);
    int kind = desc->get_kind();

    switch (kind) {
    case SYM_CLASS:
    case SYM_STRUCT:
    case SYM_UNION:
    case SYM_ENUM:
        type->set_symbol_descriptor(desc);
        break;

    case SYM_TEMPLATE:
        type->set_symbol_descriptor(desc);
        break;

    default:
        log_error_and_exit("named type not in scope but is another entity " + name);
        break;
    }
}

void TypeDescriptorLink::link_pointer_type(IndirectionType* type) {
    link_type(type->get_subtype());
}

void TypeDescriptorLink::link_reference_type(IndirectionType* type) {
    link_type(type->get_subtype());
}

void TypeDescriptorLink::link_function_type(FunctionType* type) {
    link_type_list(type->get_template_header());

    for (int i = 0; i < type->params_count(); ++i) {
        link_type(type->get_param_type(i));
    }

    link_type(type->get_return_type());
}

void TypeDescriptorLink::link_array_type(ArrayListType* type) {
    link_type(type->get_subtype());
}

void TypeDescriptorLink::link_tuple_type(TupleType* type) {
    link_type_list(type->get_types());
}

void TypeDescriptorLink::link_type_list(TypeList* types) {
    if (types == nullptr) {
        return;
    }

    for (int i = 0; i < types->types_count(); ++i) {
        link_type(types->get_type(i));
    }
}
