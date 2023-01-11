#include <iostream>
#include "printer/ir_printer.h"
#include "ir/ir_call.h"

using namespace haard;

IRPrinter::IRPrinter() {
    indent_c = 0;
    out = &functions;
}

void IRPrinter::print_modules(IRModules* modules) {
    for (int i = 0; i < modules->strings_count(); ++i) {
        *out << "%str" << i << " = " << modules->get_string(i) << "\n";
    }

    *out << '\n';

    for (int i = 0; i < modules->modules_count(); ++i) {
        print_module(modules->get_module(i));
    }
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
    IRLabel* label = (IRLabel*) ir;
    IRAlloca* alloca = (IRAlloca*) ir;

    switch (ir->get_kind()) {
    case IR_EQ:
        print_binop("cmp.eq", bin);
        break;

    case IR_NE:
        print_binop("cmp.ne", bin);
        break;

    case IR_LT:
        print_binop("cmp.lt", bin);
        break;

    case IR_GT:
        print_binop("cmp.gt", bin);
        break;

    case IR_LE:
        print_binop("cmp.le", bin);
        break;

    case IR_GE:
        print_binop("cmp.ge", bin);
        break;

    case IR_ULT:
        print_binop("cmp.ult", bin);
        break;

    case IR_UGT:
        print_binop("cmp.ugt", bin);
        break;

    case IR_ULE:
        print_binop("cmp.ule", bin);
        break;

    case IR_UGE:
        print_binop("cmp.uge", bin);
        break;

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

    case IR_LOAD64:
        *out << un->get_dst()->to_str() << " = ld ";
        *out << un->get_src()->to_str();
        break;

    case IR_LOAD32:
        *out << un->get_dst()->to_str() << " = lw ";
        *out << un->get_src()->to_str();
        break;

    case IR_LOAD16:
        *out << un->get_dst()->to_str() << " = lh ";
        *out << un->get_src()->to_str();
        break;

    case IR_LOAD8:
        *out << un->get_dst()->to_str() << " = lb ";
        *out << un->get_src()->to_str();
        break;

    case IR_STORE64:
        *out << "sd " << un->get_dst()->to_str() << ", ";
        *out << un->get_src()->to_str();
        break;

    case IR_STORE32:
        *out << "sw " << un->get_dst()->to_str() << ", ";
        *out << un->get_src()->to_str();
        break;

    case IR_STORE16:
        *out << "sh " << un->get_dst()->to_str() << ", ";
        *out << un->get_src()->to_str();
        break;

    case IR_STORE8:
        *out << "sb " << un->get_dst()->to_str() << ", ";
        *out << un->get_src()->to_str();
        break;

    case IR_ALLOCA:
        *out << alloca->get_dst()->to_str() << " = alloca ";
        *out << alloca->get_name() << ", ";
        *out << alloca->get_size() << ", " << alloca->get_align();
        break;

    case IR_RETURN:
        *out << "return";

        if (un->get_src()) {
            *out << " " << un->get_src()->to_str();
        }

        break;

    case IR_CALL:
        if (call->get_dst()) {
            *out << call->get_dst()->to_str() << " = ";
        }

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

    case IR_BZ:
        *out << "bz " << bin->get_src1()->to_str() << ", " << bin->get_src2()->to_str();
        break;

    case IR_GOTO:
        *out << "goto " << un->get_src()->to_str();
        break;

    case IR_LABEL:
        *out << "\n" << label->get_label() << ":";
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
