#include <iostream>
#include <cstring>
#include "vm/irvm.h"
#include "ir/ir_unary.h"
#include "ir/ir_bin.h"
#include "ir/ir_call.h"
#include "ir/ir_memory.h"

using namespace haard;

#define MEM_SIZE 1024 * 1024

IrVM::IrVM() {
    ip = 0;
    fp = 0;
    string_addr_counter = 0;

    mem = new uint8_t[MEM_SIZE];
    sp = (uint64_t) mem + MEM_SIZE;
}

IrVM::~IrVM() {
    delete[] mem;
}

void IrVM::execute_modules(IRModules* modules) {
    this->modules = modules;

    for (int i = 0; i < modules->strings_count(); ++i) {
        std::string s = modules->get_string(i);
        string_address_map[s] = (uint64_t) &mem[string_addr_counter];

        for (int j = 1; j < s.size() - 1; ++j) {
            char c = s[j];
            mem[string_addr_counter++] = c;
        }

        mem[string_addr_counter++] = '\0';
    }

    execute_function(modules->get_main_function());
}

void IrVM::execute_function(IRFunction* function) {
    IRLabel* label;
    IRContext* ctx = function->get_context();

    save_context();

    // FIXME: it fails when calling main function with argc, argv because
    // argc and argv are not set by the irvm/driver
    if (args.size() > 0) {
        for (int i = 0; i < function->parameters_count(); ++i) {
            values[function->get_parameter(i)->to_str()] = args[i];
        }
    }

    args.clear();

    for (int i = 0; i < ctx->instructions_count(); ++i) {
        if (ctx->get_instruction(i)->get_kind() == IR_LABEL) {
            label = (IRLabel*) ctx->get_instruction(i);
            values[label->get_label()] = i;
        }
    }

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
    IRAlloca* alloca = (IRAlloca*) ir;
    IRMemory* mem = (IRMemory*) ir;

    switch (ir->get_kind()) {
    case IR_EQ:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value == src2_value;
        ip++;
        break;

    case IR_NE:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value != src2_value;
        ip++;
        break;

    case IR_LT:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = (int64_t) src1_value < (int64_t) src2_value;
        ip++;
        break;

    case IR_GT:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = (int64_t) src1_value > (int64_t) src2_value;
        ip++;
        break;

    case IR_LE:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = (int64_t) src1_value <= (int64_t) src2_value;
        ip++;
        break;

    case IR_GE:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = (int64_t) src1_value >= (int64_t) src2_value;
        ip++;
        break;

    case IR_ULT:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value < src2_value;
        ip++;
        break;

    case IR_UGT:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value > src2_value;
        ip++;
        break;

    case IR_ULE:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value <= src2_value;
        ip++;
        break;

    case IR_UGE:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value >= src2_value;
        ip++;
        break;

    case IR_LI:
        if (un->get_src()->get_kind() == IR_VALUE_LITERAL_STRING) {
            src1_value = string_address_map[un->get_src()->get_value()];
        } else {
            src1_value = un->get_src()->to_u64();
        }

        values[un->get_dst()->to_str()] = src1_value;
        ip++;
        break;

    case IR_ADD:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value + src2_value;
        ip++;
        break;

    case IR_ADDI:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = bin->get_src2()->to_u64();
        values[bin->get_dst()->to_str()] = src1_value + src2_value;
        ip++;
        break;

    case IR_SUB:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value - src2_value;
        ip++;
        break;

    case IR_MUL:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value * src2_value;
        ip++;
        break;

    case IR_DIV:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value / src2_value;
        ip++;
        break;

    case IR_MODULO:
        src1_value = values[bin->get_src1()->to_str()];
        src2_value = values[bin->get_src2()->to_str()];
        values[bin->get_dst()->to_str()] = src1_value % src2_value;
        ip++;
        break;

    case IR_ALLOCA:
        sp = sp - alloca->get_size();
        values[alloca->get_dst()->to_str()] = sp;
        ip++;
        break;

    case IR_LOAD64:
        values[mem->get_dst()->to_str()] = load64(values[mem->get_src()->to_str()]);
        ip++;
        break;

    case IR_LOAD32:
        values[mem->get_dst()->to_str()] = load32(values[mem->get_src()->to_str()]);
        ip++;
        break;

    case IR_LOAD16:
        values[mem->get_dst()->to_str()] = load16(values[mem->get_src()->to_str()]);
        ip++;
        break;

    case IR_LOAD8:
        values[mem->get_dst()->to_str()] = load8(values[mem->get_src()->to_str()]);
        ip++;
        break;

    case IR_STORE64:
        addr = values[mem->get_dst()->to_str()];
        value = values[mem->get_src()->to_str()];
        store64(addr, value);
        ip++;
        break;

    case IR_STORE32:
        addr = values[mem->get_dst()->to_str()];
        value = values[mem->get_src()->to_str()];
        store32(addr, value);
        ip++;
        break;

    case IR_STORE16:
        addr = values[mem->get_dst()->to_str()];
        value = values[mem->get_src()->to_str()];
        store16(addr, value);
        ip++;
        break;

    case IR_STORE8:
        addr = values[mem->get_dst()->to_str()];
        value = values[mem->get_src()->to_str()];
        store8(addr, value);
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

        //execute_function(modules->get_function(call->get_name().c_str()));
        execute_function(get_function(call->get_name()));

        if (call->get_dst()) {
            values[call->get_dst()->to_str()] = return_value;
        }

        ++ip;
        break;

    case IR_GOTO:
        ip = values[un->get_src()->to_str()];
        break;

    case IR_LABEL:
        ++ip;
        break;

    case IR_BZ:
        value = values[bin->get_src1()->to_str()];

        if (value == 0) {
            ip = values[bin->get_src2()->to_str()];
        } else {
            ++ip;
        }

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
    uint64_t* ptr = (uint64_t*) addr;

    if (is_special_load_address(addr)) {
        return load_special_address(addr);
    }

    return *ptr;
}

uint64_t IrVM::load32(uint64_t addr) {
    uint32_t* ptr = (uint32_t*) addr;

    if (is_special_load_address(addr)) {
        return load_special_address(addr);
    }

    return *ptr;
}

uint64_t IrVM::load16(uint64_t addr) {
    uint16_t* ptr = (uint16_t*) addr;

    if (is_special_load_address(addr)) {
        return load_special_address(addr);
    }

    return *ptr;
}

uint64_t IrVM::load8(uint64_t addr) {
    uint8_t* ptr = (uint8_t*) addr;

    if (is_special_load_address(addr)) {
        return load_special_address(addr);
    }

    return *ptr;
}

bool IrVM::is_special_load_address(uint64_t addr) {
    switch (addr) {
    case 0x10:
        return true;
    }

    return false;
}

bool IrVM::is_special_store_address(uint64_t addr) {
    switch (addr) {
    case 0x10:
    case 0x11:
    case 0x12:
        return true;
    }

    return false;
}

uint64_t IrVM::load_special_address(uint64_t addr) {
    uint64_t v;

    switch (addr) {
    case 0x10:
        std::cin >> v;
        return v;

    default:
        break;
    }

    return 0;
}

void IrVM::store_special_address(uint64_t addr, uint64_t value) {
    switch (addr) {
    case 0x10:
        printf("%i", (int) value);
        break;

    case 0x11:
        printf("%c", (char) value);
        break;

    case 0x12:
        printf("%s", (char*) value);
        break;
    }
}

void IrVM::store64(uint64_t addr, uint64_t value) {
    uint64_t* ptr = (uint64_t*) addr;

    if (is_special_store_address(addr)) {
        store_special_address(addr, value);
    } else {
        *ptr = value;
    }
}

void IrVM::store32(uint64_t addr, uint64_t value) {
    uint32_t* ptr = (uint32_t*) addr;

    if (is_special_store_address(addr)) {
        store_special_address(addr, value);
    } else {
        *ptr = value & 0x0ffffffff;
    }
}

void IrVM::store16(uint64_t addr, uint64_t value) {
    uint16_t* ptr = (uint16_t*) addr;

    if (is_special_store_address(addr)) {
        store_special_address(addr, value);
    } else {
        *ptr = value & 0x0ffff;
    }
}

void IrVM::store8(uint64_t addr, uint64_t value) {
    uint8_t* ptr = (uint8_t*) addr;

    if (is_special_store_address(addr)) {
        store_special_address(addr, value);
    } else {
        *ptr = value & 0x0ff;
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

IRFunction* IrVM::get_function(std::string name) {
    if (function_map.count(name) == 0) {
        function_map[name] = modules->get_function(name.c_str());
    }

    return function_map[name];
}
