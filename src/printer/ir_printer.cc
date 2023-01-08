#include <iostream>
#include "printer/ir_printer.h"
#include "ir/ir_call.h"

using namespace haard;

IRPrinter::IRPrinter() {
    indent_c = 0;
    out = &functions;
}

void IRPrinter::print_module(IRModule* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        print_function(module->get_function(i));
    }

    std::cout << out->str();
}

void IRPrinter::print_function(IRFunction* function) {
    int i;

    *out << "def " << function->get_name() << "(";

    if (function->parameters_count() > 0) {
        for (i = 0; i < function->parameters_count() - 1; ++i) {
            *out << '%' << i << ", ";
        }

        *out << '%' << i;
    }

    *out << "):\n";
    indent();
    print_function_body(function);
    dedent();
    *out << "\n";
}

void IRPrinter::print_function_body(IRFunction* function) {
    IRContext* ctx = function->get_context();

    for (int i = 0; i < ctx->instructions_count(); ++i) {
        print_indentation();
        print_instruction(ctx->get_instruction(i));
        *out << '\n';
    }
}

void IRPrinter::print_instruction(IR* ir) {
    IRBin* bin = (IRBin*) ir;
    IRUnary* un = (IRUnary*) ir;
    IRCall* call = (IRCall*) ir;

    switch (ir->get_kind()) {
    case IR_ADD:
        print_binop("add", bin);
        break;

    case IR_SUB:
        print_binop("sub", bin);
        break;

    case IR_LI:
        *out << un->get_dst()->to_str() << " = li ";
        *out << un->get_src()->to_str();
        break;

    case IR_LOAD:
        *out << un->get_dst()->to_str() << " = load ";
        *out << un->get_src()->to_str();
        break;

    case IR_STORE:
        *out << "store " << un->get_dst()->to_str() << ", ";
        *out << un->get_src()->to_str();
        break;

    case IR_ALLOCA:
        *out << un->get_dst()->to_str() << " = alloca ";
        *out << un->get_src()->to_str();
        break;

    case IR_RETURN:
        *out << "return";

        if (un->get_src()) {
            *out << " " << un->get_src()->to_str();
        }

        break;

    case IR_CALL:
        *out << "call " << call->get_name() << "(";

        if (call->arguments_count() > 0) {
            int i;

            for (i = 0; i < call->arguments_count() - 1; ++i) {
                *out << call->get_argument(i)->to_str() << ", ";
            }

            *out << call->get_argument(i)->to_str();
        }

        *out << ")";
        break;

    default:
        *out << "%0 = unknown %0, %0";
        break;
    }
}

void IRPrinter::print_binop(const char* oper, IRBin* bin) {
    *out << bin->get_dst()->to_str() << " = " << oper << " ";
    *out << bin->get_src1()->to_str() << ", ";
    *out << bin->get_src2()->to_str();
}

void IRPrinter::indent() {
    indent_c++;
}

void IRPrinter::dedent() {
    indent_c--;
}

void IRPrinter::print_indentation() {
    for (int i = 0; i < indent_c; ++i) {
        *out << "    ";
    }
}
