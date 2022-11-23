#include <sstream>
#include "ir/ir_module.h"

using namespace haard;

IRModule::IRModule() {

}

IRModule::~IRModule() {
    for (int i = 0; i < functions.size(); ++i) {
        delete functions[i];
    }
}

void IRModule::add_function(IRFunction* func) {
    functions.push_back(func);
}

std::string IRModule::to_cpp() {
    std::stringstream ss;

    ss << "#include <iostream>\n";
    ss << "#include <cstdint>\n";
    ss << "typedef uint64_t u64;\n";
    ss << "typedef uint8_t u8;\n\n";

    ss << "u8* mem;\n\n";

    ss << "u64 load64(u64 addr) {\n";
    ss << "    u64 r;\n";
    ss << "    u64* p = (u64*) mem;\n";
    ss << "\n";
    ss << "    r = p[addr];\n";
    ss << "    return r;\n";
    ss << "}\n\n";

    ss << "void store64(u64 addr, u64 value) {\n";
    ss << "    u64* p = (u64*) mem;\n";
    ss << "    *p = value;\n";
    ss << "}\n\n";


    for (int i = 0; i < functions.size(); ++i) {
        ss << functions[i]->to_cpp();
        ss << '\n';
    }

    return ss.str();
}

