#include "semantic/module_functions_builder.h"
#include "semantic/function_scope_builder.h"

using namespace haard;

ModuleFunctionsBuilder::ModuleFunctionsBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void ModuleFunctionsBuilder::build_functions() {
    Module* module = get_module();

    for (int i = 0; i < module->functions_count(); ++i) {
        build_function(module->get_function(i));
    }
}

void ModuleFunctionsBuilder::build_function(Function* function) {
    /*FunctionScopeBuilder builder;

    builder.build_function(function);*/
}
