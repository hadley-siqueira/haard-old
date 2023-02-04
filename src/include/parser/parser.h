#ifndef HAARD_PARSER_H
#define HAARD_PARSER_H

#include <vector>
#include <stack>
#include "defs.h"
#include "token/token.h"
#include "scanner/scanner.h"
#include "ast/ast.h"
#include "log/logger.h"

namespace haard {
    class Parser {
        public:
            Parser();
            Parser(Logger* logger);

        public:
            Source* read(std::string path, std::string relative_path);
            Expression* read_expression_from_string(std::string str);
            Function* read_function_from_string(std::string str);
            Class* read_class_from_string(std::string str);

            std::string get_path() const;
            void set_path(const std::string &value);

            Logger* get_logger() const;
            void set_logger(Logger* value);

    private:
            Source* parse_source();
            Import* parse_import();
            Class* parse_class();
            Struct* parse_struct();
            Enum* parse_enum();
            Union* parse_union();
            Data* parse_data();

            DataField* parse_data_field();

            TypeList* parse_template_header();
            Variable* parse_class_variable();
            Function* parse_function();
            void parse_parameters(Function* function);
            Type* parse_type();
            Type* parse_primary_type();
            Type* parse_function_type();
            Type* parse_tuple_type();
            Type* parse_union_type();
            TypeList* parse_template_list();
            TypeList* parse_template_list_header();
            Statement* parse_statement();
            WhileStatement* parse_while_statement();
            ForStatement* parse_for_statement();
            void parse_for_statement_init(ForStatement* stmt);
            void parse_for_statement_condition(ForStatement* stmt);
            void parse_for_statement_increment(ForStatement* stmt);
            BranchStatement* parse_if_statement();
            BranchStatement* parse_elif_statement();
            BranchStatement* parse_else_statement();
            JumpStatement* parse_jump_statement(int tkind, int skind);
            CompoundStatement* parse_compound_statement();
            VarDeclaration* parse_variable_declaration();
            void parse_annotation();

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
            Expression* parse_string_literal();

            Expression* parse_parenthesis_or_tuple();
            Expression* parse_list_expression();
            Expression* parse_array_or_hash();
            Identifier* parse_identifier_expression();
            Expression* parse_anonymous_function();
            ExpressionList* parse_argument_list();
            ExpressionList* parse_hash(Expression* key);
            NewExpression* parse_new_expression();
            Expression* parse_delete_expression();
            Expression* parse_cast_expression();

            void advance();
            void expect(int kind);
            void expect_on_same_line(int kind);
            bool match(int kind);
            bool lookahead(int kind);
            bool lookahead(int kind, int offset);
            bool is_indentend();
            bool next_token_same_line();
            bool has_parameters();
            void indent();
            void dedent();

            std::vector<std::string> split_interpolation(std::string str);
            bool has_interpolation(std::string str);

        private:
            int idx;
            std::string path;
            std::vector<Token> tokens;
            std::stack<int> indent_stack;
            std::vector<Annotation*> annotations;
            Token matched;
            Logger* logger;
    };
}

#endif
