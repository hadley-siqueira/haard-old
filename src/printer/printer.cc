#include <iostream>
#include "printer/printer.h"

using namespace haard;
            
std::string Printer::to_str() {
    return out.str();
}

void Printer::print_source(Source* source) {
    for (int i = 0; i < source->import_count(); ++i) {
        print_import(source->get_import(i));
        out << '\n';
    }
}
            
void Printer::print_import(Import* import) {
    int i;

    out << "import ";

    if (import->path_count() > 0) {
        for (i = 0; i < import->path_count() - 1; ++i) {
            out << import->get_path(i) << '.';
        }

        out << import->get_path(i);
    }

    if (import->has_alias()) {
        out << " as " << import->get_alias();
    }
}
