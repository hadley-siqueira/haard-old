#include "semantic/module_functions_definer.h"
#include "semantic/function_declaration_scope_builder.h"

using namespace haard;

ModuleFunctionsDefiner::ModuleFunctionsDefiner(Logger* logger) {
    set_logger(logger);
}

void ModuleFunctionsDefiner::define_functions(Module* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        define_function(module->get_function(i));
    }
}

void ModuleFunctionsDefiner::define_function(Function* function) {
    FunctionDeclarationScopeBuilder builder(logger);
    builder.define_function(function);
}
