#ifndef HAARD_SCANNER_H
#define HAARD_SCANNER_H

#include <vector>
#include "token/token.h"

namespace haard {
    class Scanner {
        public:
            Scanner();
            void read_to_buffer(std::string path);

        private:
            bool has_next();
            bool lookahead(char c);
            bool is_alpha();
            bool is_num();
            bool is_alphanum();
            void reset_lexeme();
            void advance();

        private:
            int line;
            int column;
            int idx;
            int ws;
            std::vector<Token> tokens;
            std::string buffer;
            std::string lexeme;
    };
}

#endif
