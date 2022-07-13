#ifndef HAARD_SOURCE_H
#define HAARD_SOURCE_H

#include <string>

namespace haard {
    class Source {
        public:
            Source();

        public:
            std::string get_path();

            void set_path(std::string path);

        private:
            std::string path;
    };
}

#endif
