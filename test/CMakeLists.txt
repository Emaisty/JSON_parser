cmake_minimum_required(VERSION 3.27)
project(JSON_parser)

set(CMAKE_CXX_STANDARD 20)

set(PRJ ../JSON_parser)

include_directories(${PRJ}/Lexer)
include_directories(${PRJ}/JSON)
include_directories(${PRJ}/Expr)
include_directories(../lib)

set(
        SOURCE_FILES
        ${PRJ}/Lexer/lexer.cpp
        ${PRJ}/JSON/parser.cpp
        ${PRJ}/JSON/JSON.cpp
        ${PRJ}/Expr/AST_Expr.cpp


        ${PRJ}/Lexer/lexer.test.cpp
        ${PRJ}/JSON/parser.test.cpp
        ${PRJ}/JSON/JSON.test.cpp
        ${PRJ}/Expr/AST_Expr.test.cpp

        main.cpp)

add_executable(unit_tests ${SOURCE_FILES})
