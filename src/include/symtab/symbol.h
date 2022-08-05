#ifndef HAARD_SYMBOL_H
#define HAARD_SYMBOL_H

#include <string>

namespace haard {
    class Symbol {
        public:
            Symbol();
            
        public:
            int get_kind();
            std::string get_name();
            void* get_descriptor();

            void set_kind(int kind);
            void set_name(std::string name);
            void set_descriptor(void* descriptor);

        private:
            int kind;
            std::string name;
            void* descriptor;
    };
}

#endif
