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

set(SOURCES
    src/main.cc
    ${TOKEN_SRC}
    ${SCANNER_SRC}
)

add_executable(${PROJECT_NAME} ${SOURCES})
