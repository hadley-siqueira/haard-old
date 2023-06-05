#ifndef HAARD_AST_H
#define HAARD_AST_H

#include "ast/modules.h"
#include "ast/module.h"
#include "ast/import.h"
#include "ast/compound_type_descriptor.h"
#include "ast/class.h"
#include "ast/enum.h"
#include "ast/union.h"
#include "ast/struct.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/type.h"
#include "ast/indirection_type.h"
#include "ast/array_list_type.h"
#include "ast/hash_type.h"
#include "ast/tuple_type.h"
#include "ast/type_list.h"
#include "ast/named_type.h"
#include "ast/template_header.h"

// Expressions
#include "ast/expression.h"
#include "ast/identifier.h"
#include "ast/unop.h"
#include "ast/binop.h"
#include "ast/literal.h"
#include "ast/expression_list.h"
#include "ast/function_expression.h"
#include "ast/new_expression.h"
#include "ast/this_expression.h"
#include "ast/cast_expression.h"
#include "ast/string_builder.h"

// Binary operators
#include "ast/plus.h"
#include "ast/minus.h"
#include "ast/assignment.h"
#include "ast/bitwise_and.h"
#include "ast/bitwise_and_assignment.h"
#include "ast/bitwise_not_assignment.h"
#include "ast/bitwise_or.h"
#include "ast/bitwise_or_assignment.h"
#include "ast/bitwise_xor.h"
#include "ast/bitwise_xor_assignment.h"
#include "ast/division.h"
#include "ast/division_assignment.h"
#include "ast/equal.h"
#include "ast/greater_than.h"
#include "ast/greater_than_or_equal.h"
#include "ast/integer_division.h"
#include "ast/integer_division_assignment.h"
#include "ast/less_than.h"
#include "ast/less_than_or_equal.h"
#include "ast/logical_and.h"
#include "ast/logical_and_oper.h"
#include "ast/logical_or.h"
#include "ast/logical_or_oper.h"
#include "ast/minus_assignment.h"
#include "ast/modulo.h"
#include "ast/not_equal.h"
#include "ast/plus_assignment.h"
#include "ast/power.h"
#include "ast/shift_left_logical.h"
#include "ast/shift_left_logical_assignment.h"
#include "ast/shift_right_arithmetic.h"
#include "ast/shift_right_arithmetic_assignment.h"
#include "ast/shift_right_logical.h"
#include "ast/shift_right_logical_assignment.h"
#include "ast/special_assignment.h"
#include "ast/times.h"
#include "ast/times_assignment.h"
#include "ast/exclusive_range.h"
#include "ast/inclusive_range.h"
#include "ast/in.h"
#include "ast/not_in.h"
#include "ast/dot.h"
#include "ast/arrow.h"
#include "ast/call.h"
#include "ast/index.h"

#include "ast/array.h"
#include "ast/list.h"
#include "ast/hash.h"
#include "ast/tuple.h"

// Unary operators
#include "ast/address_of.h"
#include "ast/bitwise_not.h"
#include "ast/dereference.h"
#include "ast/logical_not.h"
#include "ast/logical_not_oper.h"
#include "ast/parenthesis.h"
#include "ast/pos_decrement.h"
#include "ast/pos_increment.h"
#include "ast/pre_decrement.h"
#include "ast/pre_increment.h"
#include "ast/sizeof.h"
#include "ast/unary_minus.h"
#include "ast/unary_plus.h"


// Statements
#include "ast/statement.h"
#include "ast/while_statement.h"
#include "ast/for_statement.h"
#include "ast/branch_statement.h"
#include "ast/jump_statement.h"
#include "ast/expression_statement.h"
#include "ast/compound_statement.h"
#include "ast/var_decl.h"

#endif
