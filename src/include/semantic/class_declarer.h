#ifndef HAARD_CLASS_DECLARER_H
#define HAARD_CLASS_DECLARER_H

#include "ast/ast.h"
#include "log/logger.h"

namespace haard {
    class ClassDeclarer {
        public:
            ClassDeclarer();

        public:
            void build_sources(Sources* sources);
            void build_source(Source* source);
            void build_class(Class* klass);

            Logger* get_logger() const;
            void set_logger(Logger* value);

    private:
            Scope* current_scope;
            Logger* logger;
    };
}

#endif
