cmake_minimum_required(VERSION 3.27)
project(JSON_parser)

set(CMAKE_CXX_STANDARD 20)

include_directories(JSON_parser/Lexer)
include_directories(JSON_parser/JSON)
include_directories(JSON_parser/Expr)
include_directories(lib)

set(
        SOURCE_FILES

        JSON_parser/Lexer/lexer.cpp
        JSON_parser/JSON/parser.cpp
        JSON_parser/JSON/JSON.cpp
        JSON_parser/Expr/AST_Expr.cpp

        )

set (
        TEST_FILES

        JSON_parser/Lexer/lexer.test.cpp
        JSON_parser/JSON/parser.test.cpp
        JSON_parser/JSON/JSON.test.cpp
        JSON_parser/Expr/AST_Expr.test.cpp
)

file(COPY ./test/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(JSON_parser ${SOURCE_FILES} JSON_parser/main.cpp)

add_executable(unit_tests ${SOURCE_FILES} ${TEST_FILES} test/main.cpp)
