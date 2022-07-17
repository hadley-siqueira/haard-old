cmake_minimum_required(VERSION 2.8)
project(hdc)

include_directories(src/include)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -g")

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
    src/ast/source.cc
    src/ast/import.cc
    src/ast/function.cc
    src/ast/variable.cc
    src/ast/type.cc
    src/ast/expression.cc
    src/ast/identifier.cc
    src/ast/binop.cc
    src/ast/statement.cc
    src/ast/compound_statement.cc
    src/ast/expression_statement.cc
)

set(PRINTER_SRC
    src/printer/printer.cc
)

set(DRIVER_SRC
    src/driver/driver.cc
)

set(SOURCES
    src/main.cc
    ${TOKEN_SRC}
    ${SCANNER_SRC}
    ${PARSER_SRC}
    ${AST_SRC}
    ${PRINTER_SRC}
    ${DRIVER_SRC}
)

add_executable(${PROJECT_NAME} ${SOURCES})
