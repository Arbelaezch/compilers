
cmake_minimum_required(VERSION 3.15)
project(calculator_compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(calc_compiler
    main.cpp
    lexer.cpp
    parser.cpp
    assembler.cpp
    linker.cpp)