#ifndef HAARD_AST_ANNOTATION_H
#define HAARD_AST_ANNOTATION_H

#include <string>

namespace haard {
    class Annotation {
    public:
        std::string get_value() const;
        void set_value(const std::string &value);

    private:
        std::string value;
    };
}

#endif
