cmake_minimum_required(VERSION 2.8)
project(hdc)

include_directories(src/include)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(UTILS_SRC
    src/utils/utils.cc
)

set(LOG_SRC
    src/log/log.cc
    src/log/logger.cc
    src/log/messages.cc
    src/log/info_messages.cc
    src/log/error_messages.cc
)

set(TOKEN_SRC
    src/token/token.cc
)

set(SCANNER_SRC
    src/scanner/scanner.cc
)

set(PARSER_SRC
    src/parser/parser.cc
)

set(AST_SRC
    src/ast/sources.cc
    src/ast/source.cc
    src/ast/annotation.cc
    src/ast/import.cc
    src/ast/compound_type_descriptor.cc
    src/ast/field.cc
    src/ast/class.cc
    src/ast/struct.cc
    src/ast/union.cc
    src/ast/enum.cc
    src/ast/data.cc
    src/ast/data_field.cc
    src/ast/data_field_single.cc
    src/ast/data_field_compound.cc
    src/ast/function.cc
    src/ast/variable.cc
    src/ast/type.cc
    src/ast/indirection_type.cc
    src/ast/array_list_type.cc
    src/ast/function_type.cc
    src/ast/hash_type.cc
    src/ast/type_list.cc
    src/ast/named_type.cc
    src/ast/expression.cc
    src/ast/identifier.cc
    src/ast/unop.cc
    src/ast/binop.cc
    src/ast/literal.cc
    src/ast/expression_list.cc
    src/ast/new_expression.cc
    src/ast/this_expression.cc
    src/ast/function_expression.cc
    src/ast/statement.cc
    src/ast/while_statement.cc
    src/ast/for_statement.cc
    src/ast/branch_statement.cc
    src/ast/jump_statement.cc
    src/ast/compound_statement.cc
    src/ast/expression_statement.cc
    src/ast/var_decl.cc
    src/ast/cast_expression.cc
    src/ast/string_builder.cc
)

set(IR_SRC
    src/ir/ir.cc
    src/ir/ir_bin.cc
    src/ir/ir_unary.cc
    src/ir/ir_alloca.cc
    src/ir/ir_call.cc
    src/ir/ir_label.cc
    src/ir/ir_memory.cc
    src/ir/ir_branch.cc
    src/ir/ir_value.cc
    src/ir/ir_builder.cc
    src/ir/ir_context.cc
    src/ir/ir_function.cc
    src/ir/ir_module.cc
    src/ir/ir_modules.cc
)

set(PRINTER_SRC
    src/printer/printer.cc
    src/printer/cpp_printer.cc
    src/printer/ir_printer.cc
)

set(DRIVER_SRC
    src/driver/driver.cc
)

set(SCOPE_SRC
    src/scope/symbol.cc
    src/scope/scope.cc
    src/scope/scope_builder.cc
)

set(SEMANTIC_SRC
    src/semantic/modules_scope_builder.cc
    src/semantic/module_declaration_scope_builder.cc
    src/semantic/function_declaration_scope_builder.cc
    src/semantic/type_descriptor_linker.cc
)

set(IRVM_SRC
    src/vm/irvm.cc
    src/vm/irvm_context.cc
)

set(SOURCES
    src/main.cc
    ${UTILS_SRC}
    ${LOG_SRC}
    ${TOKEN_SRC}
    ${SCANNER_SRC}
    ${PARSER_SRC}
    ${AST_SRC}
    ${IR_SRC}
    ${PRINTER_SRC}
    ${SCOPE_SRC}
    ${SEMANTIC_SRC}
    ${IRVM_SRC}
    ${DRIVER_SRC}
)

add_executable(${PROJECT_NAME} ${SOURCES})
