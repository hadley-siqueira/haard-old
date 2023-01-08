#include <iostream>
#include <cstring>
#include "vm/irvm.h"
#include "ir/ir_unary.h"
#include "ir/ir_bin.h"
#include "ir/ir_call.h"

using namespace haard;

#define MEM_SIZE 8 * 1000

IrVM::IrVM() {
    ip = 0;
    fp = 0;
    mem = new uint8_t[MEM_SIZE];
    sp = (uint64_t) mem + MEM_SIZE;
}

void IrVM::execute_module(IRModule* module) {
    this->module = module;
}

void IrVM::execute_function(IRFunction* function) {
    IRContext* ctx = function->get_context();

    save_context();

    for (int i = 0; i < function->parameters_count(); ++i) {
        values[function->get_parameter(i)->to_str()] = args[i];
    }

    args.clear();

    while (ip >= 0 && ip < ctx->instructions_count()) {
        execute(ctx->get_instruction(ip));
    }

    restore_context();
}

void IrVM::execute(IR* ir) {
    uint64_t src1_value;
    uint64_t src2_value;
    uint64_t addr;
    uint64_t value;
    IRBin* bin = (IRBin*) ir;
    IRUnary* un = (IRUnary*) ir;
    IRCall* call = (IRCall*) ir;

    switch (ir->get_kind()) {
    case IR_LI:
        //src1_value = atoi(un->get_src()->to_str().c_str());
        src1_value = un->get_src()->to_u64();
        values[un->get_dst()->to_str()] = src1_value;
        ip++;
        break;

    case IR_ADD:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value + src2_value;
        ip++;
        break;

    case IR_SUB:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value - src2_value;
        ip++;
        break;

    case IR_ALLOCA:
        sp = sp - 8;
        values[un->get_dst()->to_str()] = sp;
        ip++;
        break;

    case IR_LOAD:
        values[un->get_dst()->to_str()] = load64(values[un->get_src()->to_str()]);
        ip++;
        break;

    case IR_STORE:
        addr = values[un->get_src()->to_str()];
        value = values[un->get_dst()->to_str()];
        store64(addr, value);
        ip++;
        break;

    case IR_RETURN:
        if (un->get_src()) {
            return_value = values[un->get_src()->to_str()];
        }

        ip = -1;
        break;

    case IR_CALL:
        for (int i = 0; i < call->arguments_count(); ++i) {
            args.push_back(values[call->get_argument(i)->to_str()]);
        }

        execute_function(module->get_function(call->get_name().c_str()));

        if (call->get_dst()) {
            values[call->get_dst()->to_str()] = return_value;
        }

        ++ip;
        break;

    default:
        ip++;
        break;
    }
}

void IrVM::dump_memory(int n_bytes) {
    for (int i = MEM_SIZE - 1; i >= MEM_SIZE - n_bytes; --i) {
        printf("%x = %x\n", i, mem[i]);
    }
}

void IrVM::dump_values() {
    std::map<std::string, uint64_t>::iterator it;

    for (it = values.begin(); it != values.end(); ++it) {
        std::cout << it->first << " = " << it->second << std::endl;
    }

    std::cout << "-------------------\n";
}

uint64_t IrVM::load64(uint64_t addr) {
    uint64_t v;
    uint64_t* ptr = (uint64_t*) addr;

    switch (addr) {
    case 0x10:
        std::cin >> v;
        return v;

    default:
        break;
    }

    return *ptr;
}

void IrVM::store64(uint64_t addr, uint64_t value) {
    uint64_t* ptr = (uint64_t*) addr;

    switch (addr) {
    case 0x10:
        printf("%lu", value);
        break;

    case 0x11:
        printf("%c", (char) value);
        break;

    default:
        *ptr = value;
        break;
    }
}

void IrVM::save_context() {
    IrVMContext ctx;

    ctx.set_ip(ip);
    ctx.set_fp(fp);
    ctx.set_sp(sp);
    ctx.set_values(values);

    context_stack.push(ctx);

    ip = 0;
    values.clear();
}

void IrVM::restore_context() {
    ip = context_stack.top().get_ip();
    fp = context_stack.top().get_fp();
    sp = context_stack.top().get_sp();
    values = context_stack.top().get_values();

    context_stack.pop();
}
