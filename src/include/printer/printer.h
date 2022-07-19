#ifndef HAARD_PRINTER_H
#define HAARD_PRINTER_H

#include <sstream>
#include "ast/ast.h"

namespace haard {
    class Printer {
        public:
            Printer();

        public:
            std::string to_str();
            void print_source(Source* source);
            void print_import(Import* import);
            void print_function(Function* function);
            void print_parameters(Function* function);
            void print_type(Type* type);

            void print_statement(Statement* statement);
            void print_while_statement(WhileStatement* statement);
            void print_compound_statement(CompoundStatement* statement);
            void print_expression_statement(ExpressionStatement* statement);

            void print_expression(Expression* expression);
            void print_binop(std::string oper, BinOp* bin);
            void print_unop(std::string oper, UnOp* un);
            void print_identifier(Identifier* id);
            void print_literal(Literal* literal);
            void print_expression_list(std::string begin, std::string end, ExpressionList* tuple);
            void print_hash(ExpressionList* hash);
            void print_function_expression(FunctionExpression* function);

        private:
            void indent();
            void dedent();
            void print_indentation();

        private:
            int indent_c;
            std::stringstream out;
    };
}

#endif
