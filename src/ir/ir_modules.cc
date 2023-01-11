#include "ir/ir_modules.h"

using namespace haard;

IRModules::IRModules() {

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
