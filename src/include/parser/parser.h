#ifndef HAARD_PARSER_H
#define HAARD_PARSER_H

#include <vector>
#include <stack>
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
            Function* parse_function();
            void parse_parameters(Function* function);
            Type* parse_type();
            Statement* parse_statement();
            WhileStatement* parse_while_statement();
            CompoundStatement* parse_compound_statement();

            Expression* parse_expression();
            Expression* parse_assignment_expression();
            Expression* parse_arith_expression();
            Expression* parse_identifier_expression();

            void advance();
            void expect(int kind);
            bool match(int kind);
            bool lookahead(int kind);
            bool lookahead(int kind, int offset);
            bool is_indentend();
            bool has_parameters();
            void indent();
            void dedent();

        private:
            int idx;
            std::vector<Token> tokens;
            std::stack<int> indent_stack;
            Token matched;
    };
}

#endif
