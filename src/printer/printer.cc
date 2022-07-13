#include <iostream>
#include "printer/printer.h"

using namespace haard;

Printer::Printer() {
    indent_c = 0;
}
 
std::string Printer::to_str() {
    return out.str();
}

void Printer::print_source(Source* source) {
    for (int i = 0; i < source->import_count(); ++i) {
        print_import(source->get_import(i));
        out << '\n';
    }

    for (int i = 0; i < source->function_count(); ++i) {
        print_function(source->get_function(i));
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
            
void Printer::print_function(Function* function) {
    print_indentation();

    out << "def " << function->get_name() << " : int\n";
    indent();

    print_parameters(function);

    dedent();
}

void Printer::print_parameters(Function* function) {
    for (int i = 0; i < function->parameters_count(); ++i) {
        print_indentation();
        out << "@" << function->get_parameter(i)->get_name() << " : int\n";
    }
}

void Printer::indent() {
    indent_c++;
}

void Printer::dedent() {
    indent_c--;
}

void Printer::print_indentation() {
    for (int i = 0; i < indent_c; ++i) {
        out << "    ";
    }
}
