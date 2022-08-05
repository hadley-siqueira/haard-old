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
    if (pass == 0) {

    }
}

void SymbolTableBuilder::build_class(Class* klass) {
    if (pass == 0) {
        current_table->define(klass);
    }
}
