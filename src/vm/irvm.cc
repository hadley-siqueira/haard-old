#include <iostream>
#include "vm/irvm.h"
#include "ir/ir_unary.h"
#include "ir/ir_bin.h"

using namespace haard;

#define MEM_SIZE 8 * 5

IrVM::IrVM() {
    ip = 0;
    mem = new uint8_t[MEM_SIZE];
    sp = (uint64_t) mem + MEM_SIZE;

    values_stack.push(current_values);
    ip_stack.push(ip);
}

void IrVM::execute_function(IRFunction* function) {
    IRContext* ctx = function->get_context();

    save_context();

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

    switch (ir->get_kind()) {
    case IR_LI:
        src1_value = atoi(un->get_src()->to_str().c_str());
        current_values[un->get_dst()->to_str()] = src1_value;
        ip++;
        break;

    case IR_ADD:
        src1_value = current_values[bin->get_src1()->to_str()];
        src2_value = current_values[bin->get_src2()->to_str()];
        current_values[bin->get_dst()->to_str()] = src1_value + src2_value;
        ip++;
        break;

    case IR_SUB:
        src1_value = current_values[bin->get_src1()->to_str()];
        src2_value = current_values[bin->get_src2()->to_str()];
        current_values[bin->get_dst()->to_str()] = src1_value - src2_value;
        ip++;
        break;

    case IR_ALLOCA:
        sp = sp - 8;
        current_values[un->get_dst()->to_str()] = sp;
        ip++;
        break;

    case IR_LOAD:
        current_values[un->get_dst()->to_str()] = load64(current_values[un->get_src()->to_str()]);
        ip++;
        break;

    case IR_STORE:
        addr = current_values[un->get_src()->to_str()];
        value = current_values[un->get_dst()->to_str()];
        store64(addr, value);
        ip++;
        break;

    default:
        ip++;
        break;
    }

    dump_values();
}

void IrVM::dump_memory() {
    for (int i = MEM_SIZE - 1; i >= 0; --i) {
        printf("%x = %x\n", i, mem[i]);
    }
}

void IrVM::dump_values() {
    std::map<std::string, uint64_t>::iterator it;

    for (it = current_values.begin(); it != current_values.end(); ++it) {
        std::cout << it->first << " = " << it->second << std::endl;
    }

    std::cout << "-------------------\n";
}

uint64_t IrVM::load64(uint64_t addr) {
    uint64_t* ptr = (uint64_t*) addr;

    return *ptr;
}

void IrVM::store64(uint64_t addr, uint64_t value) {
    uint64_t* ptr = (uint64_t*) addr;

    *ptr = value;
}

void IrVM::save_context() {
    std::map<std::string, uint64_t> values;

    values_stack.push(current_values);
    ip_stack.push(ip);

    ip = 0;
    current_values = values;
}

void IrVM::restore_context() {
    ip_stack.pop();
    values_stack.pop();

    ip = ip_stack.top();
    current_values = values_stack.top();
}
