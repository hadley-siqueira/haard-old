#include "semantic/type_descriptor_linker.h"

using namespace haard;

TypeDescriptorLink::TypeDescriptorLink(Scope* scope, Logger* logger) {
    current_scope = scope;
    this->logger = logger;
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
    }
}

void TypeDescriptorLink::link_named_type(NamedType* type) {
    std::string name = type->get_name();
    Symbol* sym = current_scope->resolve(name);

    if (!sym) {
        current_scope->debug();
        logger->error_and_exit("<red>error:</red> type <white>'" + name + "'</white> not in scope");
    }

    int kind = sym->get_kind();

    switch (kind) {
    case SYM_CLASS:
    case SYM_STRUCT:
    case SYM_UNION:
    case SYM_ENUM:
    case SYM_DATA:
        type->set_symbol(sym);
        break;

    case SYM_TEMPLATE:
        type->set_symbol(sym);
        break;

    default:
        logger->error_and_exit("error: named type not in scope but is another entity");
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

Scope* TypeDescriptorLink::get_current_scope() const {
    return current_scope;
}

void TypeDescriptorLink::set_current_scope(Scope* value) {
    current_scope = value;
}

Logger* TypeDescriptorLink::get_logger() const {
    return logger;
}

void TypeDescriptorLink::set_logger(Logger* value) {
    logger = value;
}
