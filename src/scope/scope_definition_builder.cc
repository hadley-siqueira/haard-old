#include "scope/scope_definition_builder.h"
#include "log/info_messages.h"

using namespace haard;

ScopeDefinitionBuilder::ScopeDefinitionBuilder() {
    logger = nullptr;
}

void ScopeDefinitionBuilder::define_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::define_source(Source* source) {
    logger->info(info_message_defining_source(source));
}

void ScopeDefinitionBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}
