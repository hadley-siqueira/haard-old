#include "ast/source.h"

using namespace haard;

Source::Source() {

}

std::string Source::get_path() {
    return path;
}

void Source::set_path(std::string path) {
    this->path = path;
}
