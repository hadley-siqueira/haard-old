#include <sstream>
#include <iostream>
#include "ir/ir_context.h"
#include "utils/string_pool.h"

using namespace haard;

IRContext::IRContext() {
    tmp_counter = 0;
    label_counter = 0;
}

IRContext::~IRContext() {
    std::map<std::string, IRValue*>::iterator it2;

    for (int i = 0; i < instructions_count(); ++i) {
        delete instructions[i];
    }

    for (it2 = values.begin(); it2 != values.end(); ++it2) {
        delete it2->second;
    }
}

IRBin* IRContext::new_bin(int kind, IRValue* dst, IRValue* src1, IRValue* src2) {
    IRBin* ir = new IRBin(kind, dst, src1, src2);
    
    instructions.push_back(ir);
    return ir;
}

IRUnary* IRContext::new_unary(int kind,IRValue* dst, IRValue* src) {
    IRUnary* ir = new IRUnary(kind, dst, src);

    instructions.push_back(ir);
    return ir;
}

IRAlloca* IRContext::new_alloca(std::string name, int size, int align) {
    IRAlloca* alloca = new IRAlloca();
    IRValue* dst = new_temporary();

    alloca->set_dst(dst);
    alloca->set_name(name);
    alloca->set_size(size);
    alloca->set_align(align);

    alloca_map[name] = dst;

    allocas.push_back(alloca);
    return alloca;
}

IRMemory* IRContext::new_load(int size, IRValue *src) {
    IRMemory* load;
    IRValue* dst = new_temporary();

    if (size == 1) {
        load = new IRMemory(IR_LOAD8, dst, src);
    } else if (size == 2) {
        load = new IRMemory(IR_LOAD16, dst, src);
    } else if (size == 4) {
        load = new IRMemory(IR_LOAD32, dst, src);
    } else {
        load = new IRMemory(IR_LOAD64, dst, src);
    }

    add_instruction(load);
    return load;
}

IRMemory* IRContext::new_store(int size, IRValue* dst, IRValue* src) {
    IRMemory* store;

    if (size == 1) {
        store = new IRMemory(IR_STORE8, dst, src);
    } else if (size == 2) {
        store = new IRMemory(IR_STORE16, dst, src);
    } else if (size == 4) {
        store = new IRMemory(IR_STORE32, dst, src);
    } else {
        store = new IRMemory(IR_STORE64, dst, src);
    }

    add_instruction(store);
    return store;
}

IRBin* IRContext::new_binary(int kind, IRValue* src1, IRValue* src2) {
    IRBin* bin;
    IRValue* dst = new_temporary();

    bin = new IRBin(kind, dst, src1, src2);
    add_instruction(bin);

    return bin;
}

IRUnary* IRContext::new_load_immediate(int kind, std::string value) {
    IRUnary* li;
    IRValue* dst;
    IRValue* src;

    dst = new_temporary();
    src = get_literal(kind, value);
    li = new IRUnary(IR_LI, dst, src);
    add_instruction(li);

    return li;
}

IRBranch* IRContext::new_branch(int kind, IRValue* cond, IRValue* label) {
    IRBranch* branch;

    branch = new IRBranch(kind, cond, label);
    add_instruction(branch);

    return branch;
}

IRBranch* IRContext::new_branch(int kind, IRValue* label) {
    return new_branch(kind, nullptr, label);
}

IRValue* IRContext::new_temporary() {
    IRValue* v = new IRValue(IR_VALUE_TEMP, tmp_counter++);
    
    values[v->to_str()] = v;
    return v;
}

IRValue* IRContext::new_label_value(std::string value) {
    if (values.count(value) == 0) {
        values[value] = new IRValue(IR_VALUE_LABEL, value);
    }

    return values[value];
}

IRLabel* IRContext::new_label() {
    std::stringstream ss;

    IRLabel* label = new IRLabel();
    ss << "L" << label_counter;
    ++label_counter;

    label->set_label(StringPool::get(ss.str().c_str()));
    return label;
}

void IRContext::add_instruction(IR* instruction) {
    instructions.push_back(instruction);
}

IRValue* IRContext::get_literal(int kind, std::string lexeme) {
    if (values.count(lexeme) == 0) {
        values[lexeme] = new IRValue(kind, lexeme);
    }

    return values[lexeme];
}

int IRContext::instructions_count() {
    return instructions.size();
}

IR *IRContext::get_instruction(int i) {
    if (i < instructions_count()) {
        return instructions[i];
    }

    return nullptr;
}

void IRContext::set_alloca_value(std::string name, IRValue* value) {
    alloca_map[name] = value;
}

IRValue* IRContext::get_alloca_value(std::string name) {
    return alloca_map[name];
}

bool IRContext::has_alloca(std::string name) {
    return alloca_map.count(name) > 0;
}

void IRContext::move_allocas_to_instructions() {
    std::vector<IR*> tmp;

    for (int i = 0; i < allocas.size(); ++i) {
        tmp.push_back(allocas[i]);
    }

    for (int i = 0; i < instructions_count(); ++i) {
        tmp.push_back(get_instruction(i));
    }

    instructions = tmp;
}
