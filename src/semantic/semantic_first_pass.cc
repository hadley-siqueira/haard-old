#include "semantic/semantic_first_pass.h"

using namespace haard;

void SemanticFirstPass::build_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        build_module(modules->get_module(i));
    }
}

void SemanticFirstPass::build_module(Module* module) {
    set_qualified_scope(module);
    connect_sibling_scopes(module);
}

void SemanticFirstPass::set_qualified_scope(Module* module) {
    module->get_scope()->set_qualified(module->get_relative_path() + ".");
}

void SemanticFirstPass::connect_sibling_scopes(Module* module) {
    Import* import;
    Scope* scope = module->get_scope();

    for (int i = 0; i < module->import_count(); ++i) {
        import = module->get_import(i);
        scope->add_sibling(import->get_module()->get_scope());
    }
}
