#ifndef HAARD_AST_SOURCES_H
#define HAARD_AST_SOURCES_H

#include <map>
#include <vector>
#include <string>

#include "ast/source.h"

namespace haard {
    class Sources {
        public:
            Sources();
            ~Sources();

        public:
            void add_source(const char* path, Source* source);
            Source* get_source(const char* path);
            Source* get_source(int i);
            bool has_source(const char* path);

            int sources_count();

        private:
            std::vector<Source*> sources;
            std::map<const char*, Source*> sources_map;
    };
}

#endif
