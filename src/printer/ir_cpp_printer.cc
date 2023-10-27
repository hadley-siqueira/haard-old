#include <iostream>
#include "printer/ir_cpp_printer.h"
#include "ir/ir_call.h"

using namespace haard;

IRCppPrinter::IRCppPrinter() {
    indent_c = 0;
    out = &functions;
    function_counter = 0;
}

std::string IRCppPrinter::get_output() {
    return (*out).str();
}

void IRCppPrinter::print_modules(IRModules* modules) {
    *out << "#include <iostream>\n";
    *out << "#include <cstdint>\n";
    *out << "\n";
    *out << "typedef int32_t i32;\n";
    *out << "typedef uint64_t u64;\n";

    for (int i = 0; i < modules->strings_count(); ++i) {
        *out << "%str" << i << " = " << modules->get_string(i) << "\n";
    }

    *out << '\n';

    for (int i = 0; i < modules->modules_count(); ++i) {
        print_module(modules->get_module(i));
    }

    *out << "int main() {\n    return 0;\n}\n";
}

void IRCppPrinter::print_module(IRModule* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        print_function(module->get_function(i));
    }
}

void IRCppPrinter::print_function(IRFunction* function) {
    int i;

    *out << "// " << function->get_name() << "\n";
    *out << "u64 " << get_function_name(function) << "(";

    if (function->parameters_count() > 0) {
        for (i = 0; i < function->parameters_count() - 1; ++i) {
            *out << function->get_parameter(i)->to_str() << ", ";
        }

        *out << function->get_parameter(i)->to_str();
    }

    *out << ") {\n";
    indent();
    print_function_body(function);
    dedent();
    *out << "}\n";
}

void IRCppPrinter::print_function_body(IRFunction* function) {
    IRContext* ctx = function->get_context();

    for (int i = 0; i < ctx->instructions_count(); ++i) {
        print_indentation();
        print_instruction(ctx->get_instruction(i));
        *out << ";\n";
    }
}

void IRCppPrinter::print_instruction(IR* ir) {
    IRBin* bin = (IRBin*) ir;
    IRUnary* un = (IRUnary*) ir;
    IRCall* call = (IRCall*) ir;
    IRLabel* label = (IRLabel*) ir;
    IRAlloca* alloca = (IRAlloca*) ir;
    IRMemory* mem = (IRMemory*) ir;
    IRBranch* branch = (IRBranch*) ir;

    switch (ir->get_kind()) {
    case IR_EQ:
        print_binop("==", bin);
        break;

    case IR_NE:
        print_binop("!=", bin);
        break;

    case IR_LT:
        print_binop("<", bin);
        break;

    case IR_GT:
        print_binop(">", bin);
        break;

    case IR_LE:
        print_binop("<=", bin);
        break;

    case IR_GE:
        print_binop(">=", bin);
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
        print_binop("+", bin);
        break;

    case IR_ADDI:
        print_binop("+", bin);
        break;

    case IR_SUB:
        print_binop("-", bin);
        break;

    case IR_MUL:
        print_binop("*", bin);
        break;

    case IR_DIV:
        print_binop("/", bin);
        break;

    case IR_REM:
        print_binop("%", bin);
        break;

    case IR_LI:
        *out << "u64 " << un->get_dst()->to_cpp() << " = ";
        *out << un->get_src()->to_cpp();
        break;

    case IR_LOAD64:
        *out << mem->get_dst()->to_cpp() << " = ld ";
        *out << mem->get_src()->to_cpp();
        break;

    case IR_LOAD32:
        *out << "u64 " << mem->get_dst()->to_cpp() << " = *((i32*) ";
        *out << mem->get_src()->to_cpp() << ")";
        break;

    case IR_LOAD16:
        *out << mem->get_dst()->to_cpp() << " = lh ";
        *out << mem->get_src()->to_cpp();
        break;

    case IR_LOAD8:
        *out << mem->get_dst()->to_cpp() << " = lb ";
        *out << mem->get_src()->to_cpp();
        break;

    case IR_STORE64:
        *out << "sd " << mem->get_dst()->to_cpp() << ", ";
        *out << mem->get_src()->to_cpp();
        break;

    case IR_STORE32:
        *out << "*((i32*) " << mem->get_dst()->to_cpp() << ") = ";
        *out << mem->get_src()->to_cpp();
        break;

    case IR_STORE16:
        *out << "sh " << mem->get_dst()->to_cpp() << ", ";
        *out << mem->get_src()->to_cpp();
        break;

    case IR_STORE8:
        *out << "sb " << mem->get_dst()->to_cpp() << ", ";
        *out << mem->get_src()->to_cpp();
        break;

    case IR_MEMCPY:
        *out << "memcpy " << mem->get_dst()->to_cpp() << ", ";
        *out << mem->get_src()->to_cpp() << ", ";
        *out << mem->get_offset();
        break;

    case IR_ALLOCA:
        *out << "char t" << alloca->get_dst()->to_cpp() << "[";
        *out << alloca->get_size() << "];\n";
        print_indentation();

        *out << "u64 " << alloca->get_dst()->to_cpp() << " = (u64) ";
        *out << "&t" << alloca->get_dst()->to_cpp();
        break;

    case IR_RETURN:
        *out << "return";

        if (un->get_src()) {
            *out << " " << un->get_src()->to_cpp();
        }

        break;

    case IR_CALL:
        if (call->get_dst()) {
            *out << call->get_dst()->to_cpp() << " = ";
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
        *out << "bz " << branch->get_src1()->to_str() << ", " << branch->get_label()->to_str();
        break;

    case IR_BNZ:
        *out << "bnz " << branch->get_src1()->to_str() << ", " << branch->get_label()->to_str();
        break;

    case IR_GOTO:
        *out << "goto " << branch->get_label()->to_str();
        break;

    case IR_LABEL:
        *out << "\n" << label->get_label() << ":";
        break;

    default:
        *out << "%0 = unknown %0, %0";
        break;
    }
}

std::string IRCppPrinter::get_function_name(IRFunction* function) {
    std::stringstream r;

    if (fmap.count(function->get_name()) == 0) {
        fmap[function->get_name()] = function_counter;
        ++function_counter;
    }

    r << "f" << fmap[function->get_name()];
    return r.str();
}

void IRCppPrinter::print_binop(const char* oper, IRBin* bin) {
    *out << "u64 " << bin->get_dst()->to_cpp() << " = ";
    *out << bin->get_src1()->to_cpp() << " " << oper << " ";
    *out << bin->get_src2()->to_cpp();
}

void IRCppPrinter::indent() {
    indent_c++;
}

void IRCppPrinter::dedent() {
    indent_c--;
}

void IRCppPrinter::print_indentation() {
    for (int i = 0; i < indent_c; ++i) {
        *out << "    ";
    }
}
