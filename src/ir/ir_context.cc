#include <sstream>
#include <iostream>
#include "ir/ir_context.h"

using namespace haard;

IRContext::IRContext() {
    tmp_counter = 0;
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

void IRContext::add_instruction(IR* instruction) {
    instructions.push_back(instruction);
}

IRValue* IRContext::get_literal(int kind, const char* lexeme) {
    IRValue* v;

    if (values.count(lexeme) == 0) {
        values[lexeme] = new IRValue(kind, lexeme);
    }

    return values[lexeme];
}

IRValue* IRContext::get_var(Identifier* id) {
    IRValue* v;
    const char* lexeme = id->get_lexeme();

    if (values.count(lexeme) == 0) {
        values[lexeme] = new IRValue(IR_VALUE_VAR, lexeme);
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
