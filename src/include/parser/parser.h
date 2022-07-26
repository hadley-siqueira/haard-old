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
            Class* parse_class();
            Variable* parse_class_variable();
            Function* parse_function();
            void parse_parameters(Function* function);
            Type* parse_type();
            Statement* parse_statement();
            WhileStatement* parse_while_statement();
            BranchStatement* parse_if_statement();
            BranchStatement* parse_elif_statement();
            BranchStatement* parse_else_statement();
            CompoundStatement* parse_compound_statement();

            Expression* parse_expression();
            Expression* parse_assignment_expression();
            Expression* parse_range_expression();
            Expression* parse_logical_or_expression();
            Expression* parse_logical_and_expression();
            Expression* parse_equality_expression();
            Expression* parse_relational_expression();
            Expression* parse_arith_expression();
            Expression* parse_term_expression();
            Expression* parse_bitwise_or_expression();
            Expression* parse_bitwise_xor_expression();
            Expression* parse_bitwise_and_expression();
            Expression* parse_shift_expression();
            Expression* parse_unary_expression();
            Expression* parse_postfix_expression();
            Expression* parse_primary_expression();
            Expression* parse_parenthesis_or_tuple();
            Expression* parse_list_expression();
            Expression* parse_array_or_hash();
            Expression* parse_scope_expression();
            Expression* parse_identifier_expression();
            Expression* parse_anonymous_function();
            ExpressionList* parse_argument_list();
            ExpressionList* parse_hash(Expression* key);

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
