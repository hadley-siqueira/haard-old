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

IRValue* IRContext::new_temporary() {
    IRValue* v = new IRValue(IR_VALUE_TEMP, tmp_counter++);
    
    values[v->to_str()] = v;
    return v;
}

IRValue* IRContext::new_label_value(std::string value) {
    IRValue* label = new IRValue(IR_VALUE_LABEL, value);

    if (values.count(value) == 0) {
        label = new IRValue(IR_VALUE_LABEL, value);
        values[value] = label;
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
    IRValue* v;

    if (values.count(lexeme) == 0) {
        values[lexeme] = new IRValue(kind, lexeme);
    }

    return values[lexeme];
}

IRValue *IRContext::get_var(std::string name) {
    const char* lexeme = name.c_str();

    if (values.count(name) == 0) {
        values[name] = new IRValue(IR_VALUE_VAR, name);
    }

    return values[name];
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
