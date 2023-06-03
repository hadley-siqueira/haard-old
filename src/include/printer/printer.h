#ifndef HAARD_PRINTER_H
#define HAARD_PRINTER_H

#include <sstream>
#include <string>
#include "ast/ast.h"

namespace haard {
    class Printer {
        public:
            Printer();

        public:
            std::string to_str();
            void print_modules(Modules* modules);
            void print_module(Module* module);
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
            void print_identifier(Identifier* id);
            void print_literal(Literal* literal);
            void print_expression_list(std::string begin, std::string end, ExpressionList* tuple);
            void print_hash(ExpressionList* hash);
            void print_function_expression(FunctionExpression* function);
            void print_new_expression(NewExpression* expr);

            void print_plus(Plus* expr);
            void print_minus(Minus* expr);

            void print_times(Times* expr);
            void print_division(Division* expr);
            void print_integer_division(IntegerDivision* expr);
            void print_modulo(Modulo* expr);

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
