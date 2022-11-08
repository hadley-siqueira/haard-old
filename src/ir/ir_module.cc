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
    ss << "#include <ctype>\n";
    ss << "#define uint64_t u64;\n\n";

    for (int i = 0; i < functions.size(); ++i) {
        ss << functions[i]->to_cpp();
        ss << '\n';
    }

    return ss.str();
}

