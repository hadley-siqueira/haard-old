#include <iostream>
#include "semantic/identifier_scope_builder.h"
#include "log/actions.h"

using namespace haard;

IdentifierScopeBuilder::IdentifierScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void IdentifierScopeBuilder::build_identifier(Identifier* id) {
    Symbol* sym = nullptr;

    if (id->has_scope()) {
        Import* import = get_module()->get_import_with_alias(id->get_alias());

        if (import == nullptr) {
            log_error_and_exit("There is no import with alias");
        }

        Scope* scope = import->get_module()->get_scope();
        sym = scope->local_has(id->get_name());
    } else {
        sym = get_scope()->resolve(id->get_name());
    }

    if (!sym) {
        log_info("scope: " + get_scope()->debug());
        //log_error_and_exit(error_message_id_not_in_scope(get_module(), id));
        DBG; exit(0);
    }

    id->set_symbol(sym);

    if (id->has_template()) {
        //link_template_header(id->get_template_header());

        if (!sym->has_template(id->get_template_header())) {
            //generate_templates(id);
            sym->add_template(id->get_template_header()->get_types());
        }
    }
}
