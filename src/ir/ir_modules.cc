#include "ir/ir_modules.h"

using namespace haard;

IRModules::IRModules() {
    string_counter = 0;
}

IRModules::~IRModules() {
    for (int i = 0; i < modules_count(); ++i) {
        delete modules[i];
    }
}

void IRModules::add_module(IRModule* module) {
    modules.push_back(module);

    if (module->get_main_function()) {
        main_function = module->get_main_function();
    }
}

IRModule *IRModules::get_module(int i) {
    if (i < modules_count()) {
        return modules[i];
    }

    return nullptr;
}

int IRModules::modules_count() {
    return modules.size();
}

IRFunction* IRModules::get_function(const char* name) {
    IRFunction* function;

    for (int i = 0; i < modules_count(); ++i) {
        function = modules[i]->get_function(name);

        if (function) {
            return function;
        }
    }

    return nullptr;
}

IRFunction* IRModules::get_main_function() const {
    return main_function;
}

void IRModules::add_string_literal(std::string s) {
    if (string_pool_map.count(s) == 0) {
        string_pool_map[s] = string_counter;
        string_counter++;
        string_pool.push_back(s);
    }
}

int IRModules::strings_count() {
    return string_pool.size();
}

std::string IRModules::get_string(int i) {
    return string_pool[i];
}
