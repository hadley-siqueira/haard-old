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

            void print_cast(CastExpression* expr);

            void print_logical_or(LogicalOr* expr);
            void print_logical_or_oper(LogicalOrOper* expr);
            void print_logical_and(LogicalAnd* expr);
            void print_logical_and_oper(LogicalAndOper* expr);

            void print_equal(Equal* expr);
            void print_not_equal(NotEqual* expr);

            void print_less_than(LessThan* expr);
            void print_greater_than(GreaterThan* expr);
            void print_less_than_or_equal(LessThanOrEqual* expr);
            void print_greater_than_or_equal(GreaterThanOrEqual* expr);

            void print_in(In* expr);
            void print_not_in(NotIn* expr);

            void print_inclusive_range(InclusiveRange* expr);
            void print_exclusive_range(ExclusiveRange* expr);

            void print_plus(Plus* expr);
            void print_minus(Minus* expr);

            void print_times(Times* expr);
            void print_division(Division* expr);
            void print_integer_division(IntegerDivision* expr);
            void print_modulo(Modulo* expr);

            void print_power(Power* expr);

            void print_bitwise_or(BitwiseOr* expr);
            void print_bitwise_xor(BitwiseXor* expr);
            void print_bitwise_and(BitwiseAnd* expr);

            void print_shift_left_logical(ShiftLeftLogical* expr);
            void print_shift_right_logical(ShiftRightLogical* expr);
            void print_shift_right_arithmetic(ShiftRightArithmetic* expr);

            void print_logical_not_oper(LogicalNotOper* expr);
            void print_logical_not(LogicalNot* expr);
            void print_address_of(AddressOf* expr);
            void print_dereference(Dereference* expr);
            void print_bitwise_not(BitwiseNot* expr);
            void print_unary_minus(UnaryMinus* expr);
            void print_unary_plus(UnaryPlus* expr);
            void print_pre_increment(PreIncrement* expr);
            void print_pre_decrement(PreDecrement* expr);
            void print_pos_increment(PosIncrement* expr);
            void print_pos_decrement(PosDecrement* expr);
            void print_sizeof(Sizeof* expr);
            void print_parenthesis(Parenthesis* expr);
            void print_call(Call* expr);

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
