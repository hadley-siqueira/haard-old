#ifndef HAARD_AST_SOURCE_H
#define HAARD_AST_SOURCE_H

#include <string>
#include <vector>
#include "ast/import.h"

namespace haard {
    class Source {
        public:
            Source();
            ~Source();

        public:
            std::string get_path();
            Import* get_import(int idx);

            void set_path(std::string path);
            void add_import(Import* import);
            
            int import_count();

        private:
            std::string path;
            std::vector<Import*> imports;
    };
}

#endif
