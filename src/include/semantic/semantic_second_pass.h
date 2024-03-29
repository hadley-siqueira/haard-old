#ifndef HAARD_SEMANTIC_SECOND_PASS_H
#define HAARD_SEMANTIC_SECOND_PASS_H

#include "ast/ast.h"
#include "semantic/semantic_pass.h"

namespace haard {
    class SemanticSecondPass : SemanticPass {
    public:
        void build_modules(Modules* modules);
        void build_module(Module* module);

        void build_class(Class* klass);

        void build_function(Function* function);
        void build_method(Function* method);

        void build_statement(Statement* stmt);
        void build_compound_statement(CompoundStatement* stmt);
        void build_while_statement(WhileStatement* stmt);
        void build_if_statement(If* stmt);
        void build_elif_statement(Elif* stmt);
        void build_else_statement(Else* stmt);
        void build_return_statement(JumpStatement* stmt);
        void build_var_declaration_statement(VarDeclaration* stmt);
        void build_expression_statement(ExpressionStatement* stmt);

        void build_expression(Expression* expr);

        void build_assignment(Assignment* expr);

        void build_equal(Equal* expr);
        void build_not_equal(NotEqual* expr);
        void build_less_than(LessThan* expr);

        void build_plus(Plus* expr);
        void build_minus(Minus* expr);

        void build_times(Times* expr);
        void build_division(Division* expr);

        void build_bitwise_or(BitwiseOr* expr);
        void build_bitwise_xor(BitwiseXor* expr);
        void build_bitwise_and(BitwiseAnd* expr);

        void print_shift_left_logical(ShiftLeftLogical* expr);
        void print_shift_right_logical(ShiftRightLogical* expr);
        void print_shift_right_arithmetic(ShiftRightArithmetic* expr);

        void build_call(Call* expr);
        void build_method_call(Call* expr);
        void build_constructor_call(Call* expr);

        void build_expression_list(ExpressionList* list);

        void build_address_of(AddressOf* expr);
        void build_dereference(Dereference* expr);
        void build_index(Index* expr);
        void build_dot(Dot* expr);
        void build_cast(Cast* expr);

        void build_pre_increment(PreIncrement* expr);
        void build_pre_decrement(PreDecrement* expr);
        void build_pos_increment(PosIncrement* expr);
        void build_pos_decrement(PosDecrement* expr);
        void build_parenthesis(Parenthesis* expr);

        void build_identifier(Identifier* expr);
        void build_template_header(TemplateHeader* theader);
        void build_this(This* expr);
        void build_literal_bool(Literal* expr);
        void build_literal_char(Literal* expr);
        void build_literal_integer(Literal* expr);
        void build_literal_float(Literal* expr);
        void build_literal_double(Literal* expr);
        void build_literal_string(Literal* expr);
        void build_string_builder(StringBuilder* expr);

    private:
        bool is_new_variable(Assignment* expr);
        bool is_simple_call(Call* expr);
        bool is_member_call(Call* expr);
        bool is_constructor_call(Call* expr);
        void create_variable(Assignment* expr);
        SymbolDescriptor* find_best_match(Symbol* sym, ExpressionList* args, TemplateHeader* templates=nullptr);
        bool compare_match(Function *function, ExpressionList* args);
        void build_simple_call(Call* expr);
        void build_member_call(Call* expr);
        void set_call_type(Call* expr, SymbolDescriptor *desc);
        void add_members_initialization(Function* function);
        void add_parent_constructor_call(Function* function);

    };
}

#endif
