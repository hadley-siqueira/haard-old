#include "vm/irvm.h"
#include "ir/ir_unary.h"
#include "ir/ir_bin.h"

using namespace haard;

IrVM::IrVM() {
    mem = new uint8_t[1 * 1024 * 1024];
}

void IrVM::execute_function(IRFunction* function) {
    IRContext* ctx = function->get_context();

    ip = 0;

    while (ip < ctx->instructions_count()) {
        execute(ctx->get_instruction(ip));
    }
}

void IrVM::execute(IR* ir) {
    uint64_t src1_value;
    uint64_t src2_value;
    IRBin* bin = (IRBin*) ir;
    IRUnary* un = (IRUnary*) ir;

    switch (ir->get_kind()) {
    case IR_LI:
        src1_value = atoi(un->get_src()->to_str().c_str());
        current_values->at(un->get_dst()->to_str()) = src1_value;
        ip++;
        break;

    case IR_ADD:
        src1_value = current_values->at(bin->get_src1()->to_str());
        src2_value = current_values->at(bin->get_src2()->to_str());
        current_values->at(bin->get_dst()->to_str()) = src1_value + src2_value;
        ip++;
        break;

    case IR_SUB:
        src1_value = current_values->at(bin->get_src1()->to_str());
        src2_value = current_values->at(bin->get_src2()->to_str());
        current_values->at(bin->get_dst()->to_str()) = src1_value - src2_value;
        ip++;
        break;

    default:
        ip++;
        break;
    }
}

uint64_t IrVM::load64(uint64_t addr) {
    uint64_t* ptr = (uint64_t*) mem;

    return ptr[addr];
}

void IrVM::store64(uint64_t addr, uint64_t value) {
    uint64_t* ptr = (uint64_t*) mem;

    ptr[addr] = value;
}

void IrVM::save_context() {
    //std::map<std::string, unt64_t> values;
    //ip_stack.push(ip);

}

void IrVM::restore_context() {
    ip_stack.pop();
    values_stack.pop();

    ip = ip_stack.top();
    current_values = &values_stack.top();
}
