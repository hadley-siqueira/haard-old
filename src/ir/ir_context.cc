#include <sstream>
#include <iostream>
#include "ir/ir_context.h"

using namespace haard;

IRContext::IRContext() {
    tmp_counter = 0;
}

IRContext::~IRContext() {
    std::list<IR*>::iterator it;
    std::map<std::string, IRValue*>::iterator it2;

    for (it = instructions.begin(); it != instructions.end(); ++it) {
        delete *it;
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

void IRContext::debug() {
    std::list<IR*>::iterator it;

    for (it = instructions.begin(); it != instructions.end(); ++it) {
        IR* inst = *it;
        std::cout << inst->to_str() << std::endl;
    }
}

std::string IRContext::to_str() {
    std::stringstream ss;
    std::list<IR*>::iterator it;

    for (it = instructions.begin(); it != instructions.end(); ++it) {
        IR* inst = *it;

        if (inst->get_kind() != IR_LABEL) {
            ss << "    ";
        }

        ss << inst->to_str() << '\n';
    }

    return ss.str();
}

std::string IRContext::to_cpp() {
    std::stringstream ss;
    std::list<IR*>::iterator it;

    for (it = instructions.begin(); it != instructions.end(); ++it) {
        IR* inst = *it;

        if (inst->get_kind() != IR_LABEL) {
            ss << "    ";
        }

        ss << inst->to_cpp() << '\n';
    }

    return ss.str();
}
