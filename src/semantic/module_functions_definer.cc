#include "semantic/module_functions_definer.h"
#include "semantic/function_declaration_scope_builder.h"

using namespace haard;

ModuleFunctionsDefiner::ModuleFunctionsDefiner(ScopeBuilderContext* context) {
    set_context(context);
}

void ModuleFunctionsDefiner::define_functions() {
    Module* module = get_module();

    for (int i = 0; i < module->functions_count(); ++i) {
        define_function(module->get_function(i));
    }
}

void ModuleFunctionsDefiner::define_function(Function* function) {
    FunctionDeclarationScopeBuilder builder(get_context());

    builder.define_function(function);
}
