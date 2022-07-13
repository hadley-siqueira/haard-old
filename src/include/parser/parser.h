#ifndef HAARD_PARSER_H
#define HAARD_PARSER_H

#include <vector>
#include "defs.h"
#include "token/token.h"
#include "scanner/scanner.h"
#include "ast/ast.h"

namespace haard {
    class Parser {
        public:
            Parser();

        public:
            Source* read(std::string path);

        private:
            Source* parse_source();
            Import* parse_import();

            void advance();
            void expect(int kind);
            bool match(int kind);
            bool lookahead(int kind);

        private:
            int idx;
            std::vector<Token> tokens;
            Token matched;
    };
}

#endif
