#ifndef HAARD_CPP_PRINTER_H
#define HAARD_CPP_PRINTER_H

#include <sstream>
#include <string>
#include <set>
#include <map>
#include <stack>
#include "ast/ast.h"

namespace haard {
    class CppPrinter {
        public:
            CppPrinter();

        public:
            std::string to_str();
            void print_sources(Sources* sources);
            void print_source(Source* source);
            void print_import(Import* import);
            void print_class(Class* klass);
            void print_function(Function* function);
            void print_parameters(Function* function);
            void print_type(Type* type);
            void print_type_list(std::string oper, TypeList* tlist);

            void print_statement(Statement* statement);
            void print_while_statement(WhileStatement* statement);
            void print_for_statement(ForStatement* statement);
            void print_branch_statement(BranchStatement* statement);
            void print_jump_statement(std::string op, JumpStatement* statement);
            void print_compound_statement(CompoundStatement* statement);
            void print_expression_statement(ExpressionStatement* statement);
            void print_variable_declaration(VarDeclaration* decl);

            void print_expression(Expression* expression);
            void print_binop(std::string oper, BinOp* bin);
            void print_unop(std::string oper, UnOp* un, bool before=true);
            void print_double_dolar(UnOp* un);
            void print_identifier(Identifier* id);
            void print_literal(Literal* literal);
            void print_literal_symbol(Literal* literal);
            void print_expression_list(std::string begin, std::string end, ExpressionList* tuple);
            void print_hash(ExpressionList* hash);
            void print_function_expression(FunctionExpression* function);
            void print_new_expression(NewExpression* expr);
            void print_delete_expression(UnOp* expr);
            void print_delete_array_expression(UnOp* expr);

        private:
            void indent();
            void dedent();
            void print_indentation();
            void generate_symbols(std::stringstream& out);
            Function* main_function;

        private:
            int indent_c;
            int symbol_counter;
            std::stringstream out;
            std::stringstream tail;
            std::stringstream signatures;
            std::map<std::string, int> symbol_map;
    };
}

#endif
