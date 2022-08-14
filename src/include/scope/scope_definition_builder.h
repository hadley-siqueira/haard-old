#ifndef HAARD_SCOPE_DEFINITION_BUILDER_H
#define HAARD_SCOPE_DEFINITION_BUILDER_H

#include "ast/ast.h"
#include "log/logger.h"

namespace haard {
    class ScopeDefinitionBuilder {
        public:
            ScopeDefinitionBuilder();

        public:
            void define_sources(Sources* sources);
            void define_source(Source* source);

        public:
            void set_logger(Logger* logger);

        private:
            Logger* logger;
    };
}

#endif
