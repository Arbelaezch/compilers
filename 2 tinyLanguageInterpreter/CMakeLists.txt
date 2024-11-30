cmake_minimum_required(VERSION 3.12)
project(TinyInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tiny_interpreter
    src/main.cpp
    src/lexer.cpp
    src/token.cpp
    src/ast.cpp
    src/parser.cpp
)

target_include_directories(tiny_interpreter PRIVATE include)