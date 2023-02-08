#ifndef HAARD_AST_SOURCES_H
#define HAARD_AST_SOURCES_H

#include <map>
#include <vector>
#include <string>

#include "ast/source.h"

namespace haard {
    class Modules {
    public:
        Modules();
        ~Modules();

    public:
        void add_module(std::string path, Module* module);
        Module* get_module(std::string path);
        Module* get_module(int i);
        bool has_module(std::string path);

        int modules_count();

    private:
        std::vector<Module*> modules;
        std::map<std::string, Module*> modules_map;
    };
}

#endif
