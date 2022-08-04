#ifndef HAARD_CPP_PRINTER_H
#define HAARD_CPP_PRINTER_H

#include <sstream>
#include <string>
#include <set>
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
            void print_identifier(Identifier* id);
            void print_literal(Literal* literal);
            void print_expression_list(std::string begin, std::string end, ExpressionList* tuple);
            void print_hash(ExpressionList* hash);
            void print_function_expression(FunctionExpression* function);

        private:
            void indent();
            void dedent();
            void print_indentation();
            void save_scope();
            void restore_scope();
            void define(std::string name);

        private:
            int indent_c;
            std::stringstream out;
            std::set<std::string> current_scope;
            std::stack<std::set<std::string> > scopes;
    };
}

#endif
