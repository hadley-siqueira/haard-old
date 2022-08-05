#include <iostream>
#include "symtab/symbol_table_builder.h"

using namespace haard;

SymbolTableBuilder::SymbolTableBuilder() {
    pass = 0;
    current_table = nullptr;
}

void SymbolTableBuilder::build_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }

    pass++;
}

void SymbolTableBuilder::build_source(Source* source) {
    current_table = source->get_symbol_table();

    if (pass == 0) {
        for (int i = 0; i < source->classes_count(); ++i) {
            build_class(source->get_class(i));
        }

        for (int i = 0; i < source->function_count(); ++i) {
            build_function(source->get_function(i));
        }
    }
}

void SymbolTableBuilder::build_class(Class* klass) {

    if (pass == 0) {
        if (!current_table->has(klass->get_name())) {
            current_table->define(klass);
        } else {
            Symbol* sym = current_table->has(klass->get_name());
            std::cout << "Error: you tried to define a class named '" << klass->get_name() << "', but it is already defined. First occurrence\n";
            exit(0);
        }
    }
}

void SymbolTableBuilder::build_function(Function* func) {
    Symbol* sym = current_table->has(func->get_name());

    if (!sym) {
        current_table->define(func);
    } else if (sym->get_kind() == SYM_FUNCTION) {
            sym->add_descriptor(func);
    } else {
            std::cout << "Error: you tried to define a function named '" << func->get_name() << "', but it is already defined. First occurrence\n";
            exit(0);
    }
}

