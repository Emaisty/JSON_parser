cmake_minimum_required(VERSION 3.27)
project(JSON_parser)

set(CMAKE_CXX_STANDARD 20)

include_directories(JSON_parser/Lexer)
include_directories(JSON_parser/JSON)
include_directories(JSON_parser/Expr)

set(
        SOURCE_FILES

        JSON_parser/Lexer/lexer.cpp
        JSON_parser/JSON/parser.cpp
        JSON_parser/JSON/JSON.cpp
        JSON_parser/Expr/AST_Expr.cpp

        JSON_parser/main.cpp)

add_executable(JSON_parser ${SOURCE_FILES})
