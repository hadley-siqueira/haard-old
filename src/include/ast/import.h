#ifndef HAARD_AST_IMPORT_H
#define HAARD_AST_IMPORT_H

#include <vector>
#include <string>
#include "token/token.h"
#include "ast/module.h"

namespace haard {
    class Module;

    class Import {
        public:
            Import();

        public:
            int get_line();
            int get_column();
            std::string get_alias();
            std::vector<std::string> get_path();
            std::string get_path(int idx);
            Module* get_module();

            void set_line(int line);
            void set_column(int column);
            void set_alias(std::string alias);
            void set_path(std::vector<std::string> path);
            void set_from_token(Token& token);
            void set_module(Module* module);

            void add_to_path(std::string s);
            bool has_alias();
            int path_count();

        private:
            int line;
            int column;
            std::string alias;
            std::vector<std::string> path;
            Module* module;
    };
}

#endif
