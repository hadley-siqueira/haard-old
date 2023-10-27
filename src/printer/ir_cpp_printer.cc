#include <iostream>
#include "printer/ir_cpp_printer.h"
#include "ir/ir_call.h"

using namespace haard;

IRCppPrinter::IRCppPrinter() {
    indent_c = 0;
    function_counter = 0;
}

std::string IRCppPrinter::get_output() {
    std::stringstream output;

    output << headers.str();
    output << functions.str();

    return output.str();
}

void IRCppPrinter::print_modules(IRModules* modules) {
    print_headers();
    /*for (int i = 0; i < modules->strings_count(); ++i) {
        *out << "%str" << i << " = " << modules->get_string(i) << "\n";
    }

    *out << '\n';*/

    for (int i = 0; i < modules->modules_count(); ++i) {
        print_module(modules->get_module(i));
    }

    print_main_function();
}

void IRCppPrinter::print_module(IRModule* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        print_function(module->get_function(i));
    }
}

void IRCppPrinter::print_function(IRFunction* function) {
    int i;
    bool is_syscall = false;
    out = &functions;

    if (function->get_name().find("syscall") != std::string::npos) {
        is_syscall = true;
    } else if (function->get_name().find("main") != std::string::npos) {
        main_function = function;
    }

    *out << "// " << function->get_name() << "\n";
    *out << "u64 " << get_function_name(function) << "(";

    if (function->parameters_count() > 0) {
        for (i = 0; i < function->parameters_count() - 1; ++i) {
            *out << "u64 " << function->get_parameter(i)->to_cpp() << ", ";
        }

        *out << "u64 " << function->get_parameter(i)->to_cpp();
    }

    *out << ") {\n";
    indent();

    if (is_syscall) {
        print_syscall_body();
    } else {
        print_function_body(function);
    }

    dedent();
    *out << "}\n\n";
}

void IRCppPrinter::print_function_body(IRFunction* function) {
    IRContext* ctx = function->get_context();

    print_function_temporaries(function);
    int i;

    for (int i = 0; i < ctx->instructions_count(); ++i) {
        print_indentation();
        auto inst = ctx->get_instruction(i);
        print_instruction(ctx->get_instruction(i));

        if (inst->get_kind() != IR_LABEL) {
            *out << ";";
        }

        *out << "\n";
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
        *out << un->get_dst()->to_cpp() << " = ";
        *out << un->get_src()->to_cpp();
        break;

    case IR_LOAD64:
        *out << mem->get_dst()->to_cpp() << " = ld ";
        *out << mem->get_src()->to_cpp();
        break;

    case IR_LOAD32:
        *out << mem->get_dst()->to_cpp() << " = *((i32*) ";
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
        *out << alloca->get_dst()->to_cpp() << " = (u64) ";
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

        *out << get_function_name(call->get_name()) << "(";

        if (call->arguments_count() > 0) {
            int i;

            for (i = 0; i < call->arguments_count() - 1; ++i) {
                *out << call->get_argument(i)->to_cpp() << ", ";
            }

            *out << call->get_argument(i)->to_cpp();
        }

        *out << ")";
        break;

    case IR_BZ:
        *out << "if (" << branch->get_src1()->to_cpp() << " == 0) goto " << branch->get_label()->to_str();
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

std::string IRCppPrinter::get_function_name(std::string name) {
    std::stringstream r;
    r << "f" << fmap[name];
    return r.str();
}

void IRCppPrinter::print_binop(const char* oper, IRBin* bin) {
    *out << bin->get_dst()->to_cpp() << " = ";
    *out << bin->get_src1()->to_cpp() << " " << oper << " ";
    *out << bin->get_src2()->to_cpp();
}

void IRCppPrinter::print_headers() {
    headers << "#include <iostream>\n";
    headers << "#include <cstdint>\n";
    headers << "\n";
    headers << "typedef int32_t i32;\n";
    headers << "typedef uint64_t u64;\n";
}

void IRCppPrinter::print_main_function() {
    out = &functions;
    *out << "\nint main() {\n";

    if (main_function) {
        *out << "    " << get_function_name(main_function) << "();\n";
    }

    *out << "    return 0;\n}\n";
}

void IRCppPrinter::print_syscall_body() {
    *out << "    switch (t0) {\n"
            "    case 0:\n"
            "        std::cout << t2 << std::endl;\n"
            "        break;\n"
            "    }\n";
}

void IRCppPrinter::print_function_temporaries(IRFunction* function) {
    int i;

    if (function->temp_count() == 0) {
        return;
    }

    *out << "    u64 ";
    for (i = 0; i < function->temp_count() - 1; ++i) {
        IRValue* tmp = function->get_temp(i);

        if (is_not_parameter(function, tmp)) {
            *out << function->get_temp(i)->to_cpp() << ", ";
        }
    }

    *out << function->get_temp(i)->to_cpp() << ";\n";
}

bool IRCppPrinter::is_not_parameter(IRFunction *function, IRValue *value) {
    for (int i = 0; i < function->parameters_count(); ++i) {
        if (function->get_parameter(i) == value) {
            return false;
        }
    }

    return true;
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
