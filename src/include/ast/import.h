#ifndef HAARD_AST_IMPORT_H
#define HAARD_AST_IMPORT_H

#include <vector>
#include <string>
#include "token/token.h"
#include "ast/source.h"

namespace haard {
    class Source;

    class Import {
        public:
            Import();

        public:
            int get_line();
            int get_column();
            const char* get_alias();
            std::vector<const char*> get_path();
            const char* get_path(int idx);
            Source* get_source();

            void set_line(int line);
            void set_column(int column);
            void set_alias(const char* alias);
            void set_path(std::vector<const char*> path);
            void set_from_token(Token& token);
            void set_source(Source* source);

            void add_to_path(const char* s);
            bool has_alias();
            int path_count();

        private:
            int line;
            int column;
            const char* alias;
            std::vector<const char*> path;
            Source* source;
    };
}

#endif
