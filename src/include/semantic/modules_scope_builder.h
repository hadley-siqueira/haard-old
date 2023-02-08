#ifndef HAARD_MODULES_SCOPE_BUILDER_H
#define HAARD_MODULES_SCOPE_BUILDER_H

#include "ast/sources.h"
#include "log/logger.h"

namespace haard {
    class ModulesScopeBuilder {
        public:
            void build_modules(Modules* modules);

            Logger* get_logger() const;
            void set_logger(Logger* value);

    private:
            Logger* logger;
    };
}

#endif
