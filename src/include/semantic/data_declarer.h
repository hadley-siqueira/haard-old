#ifndef HAARD_DATA_DECLARER_H
#define HAARD_DATA_DECLARER_H

#include "ast/ast.h"
#include "log/logger.h"

namespace haard {
    class DataDeclarer {
        public:
            DataDeclarer();

        public:
            void build_sources(Sources* sources);
            void build_source(Source* source);
            void build_data(Data* data);

            Logger* get_logger() const;
            void set_logger(Logger* value);

    private:
            Scope* current_scope;
            Logger* logger;
    };
}

#endif
