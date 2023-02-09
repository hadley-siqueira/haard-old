#ifndef HAARD_AST_H
#define HAARD_AST_H

#include "ast/modules.h"
#include "ast/module.h"
#include "ast/import.h"
#include "ast/compound_type_descriptor.h"
#include "ast/class.h"
#include "ast/data.h"
#include "ast/data_field.h"
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
