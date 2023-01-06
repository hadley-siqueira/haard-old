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
    functions_map[func->get_name()] = func;
}

int IRModule::functions_count() {
    return functions.size();
}

IRFunction* IRModule::get_function(int i) {
    if (i < functions_count()) {
        return functions[i];
    }

    return nullptr;
}

IRFunction* IRModule::get_function(const char* name) {
    if (functions_map.count(name) > 0) {
        return functions_map[name];
    }

    return nullptr;
}
