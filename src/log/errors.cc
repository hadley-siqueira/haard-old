#include <sstream>
#include <fstream>
#include "log/errors.h"
#include "log/utils.h"

namespace haard {
    std::string error_unexpected_token(std::string path, Token& token) {
        std::stringstream ss;
        int line = token.get_line();
        int column = token.get_column();
        int count = token.get_lexeme().size();

        ss << "<red>error</red>: unexpected token\n";
        ss << foobar(path, line, column, count);

        return ss.str();
    }
}
